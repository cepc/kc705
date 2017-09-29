#include <Windows.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

/*********************************************************************
 *                                                                   *
 *                 D E C L A R A T I O N S                           *
 *                                                                   *
 *********************************************************************/

// Windows equivalents for gcc builtin atomic operations
#define __sync_add_and_fetch(x,y) (InterlockedExchangeAdd(x, (y)) + (y))
#define __sync_sub_and_fetch(x,y) (InterlockedExchangeAdd(x, -(y)) - (y))

struct xillyfifo {
  LONG read_total;
  LONG write_total;
  LONG bytes_in_fifo;
  unsigned int read_position;
  unsigned int write_position;
  unsigned int size;
  unsigned int done;
  unsigned char *baseaddr;
  HANDLE write_event;
  HANDLE read_event;   
};

struct xillyinfo {
  int slept;
  int bytes;
  int position;
  void *addr;
};

#define FIFO_BACKOFF 0
static int read_fd = 0;

/*********************************************************************
 *                                                                   *
 *                 A P I   F U N C T I O N S                         *
 *                                                                   *
 *********************************************************************/

// IMPORTANT:
// =========
//
// NEITHER of the fifo_* functions is reentrant. Only one thread should have
// access to any set of them. This is pretty straightforward when one thread
// writes and one thread reads from the FIFO.
//
// Also make sure that fifo_drained() and fifo_wrote() are NEVER called with
// req_bytes larger than what their request-counterparts RETURNED, or
// things will go crazy pretty soon.


void errorprint(char *what, DWORD dw) {
  LPVOID lpMsgBuf;
  
  FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		0, NULL );
  
  
  fprintf(stderr, "%s: Error=%08x:\n%s\n",
	  what, dw, lpMsgBuf); 
  
  LocalFree(lpMsgBuf);
}

int fifo_init(struct xillyfifo *fifo,
	      unsigned int size) {

  fifo->baseaddr = NULL;
  fifo->size = 0;
  fifo->bytes_in_fifo = 0;
  fifo->read_position = 0;
  fifo->write_position = 0;
  fifo->read_total = 0;
  fifo->write_total = 0;
  fifo->done = 0;

  // No security attributes, autoreset, unnamed
  fifo->read_event = CreateEvent(NULL, FALSE, FALSE, NULL); // Initially nonsignaled
  fifo->write_event = CreateEvent(NULL, FALSE, TRUE, NULL); // Initially signaled
   
  fifo->baseaddr = malloc(size);

  if (!fifo->baseaddr)
    return -1;

  if (!VirtualLock(fifo->baseaddr, size)) {    
    unsigned int i;
    unsigned char *buf = fifo->baseaddr;

    errorprint("Failed to lock RAM, so FIFO's memory may swap to disk", GetLastError());
    
    // Write something every 1024 bytes (4096 should be OK, actually).
    // Hopefully all pages are in real RAM after this. Better than nothing.

    for (i=0; i<size; i+=1024)
      buf[i] = 0;
  }

  fifo->size = size;

  return 0; // Success
}

void fifo_done(struct xillyfifo *fifo) {
  fifo->done = 1;
  
  if (!SetEvent(fifo->read_event))
    errorprint("fifo_done: Failed to set read event", GetLastError());

  if (!SetEvent(fifo->write_event))
    errorprint("fifo_done: Failed to set write event", GetLastError());
}

void fifo_destroy(struct xillyfifo *fifo) {
  if (!fifo->baseaddr)
    return; // Better safe than SEGV

  VirtualUnlock(fifo->baseaddr, fifo->size);
  free(fifo->baseaddr);
  
  CloseHandle(fifo->read_event);
  CloseHandle(fifo->write_event);

  fifo->baseaddr = NULL;
}

int fifo_request_drain(struct xillyfifo *fifo,
		       struct xillyinfo *info) {
  int taken = 0;
  unsigned int now_bytes, max_bytes;

  info->slept = 0;
  info->addr = NULL;

  now_bytes = __sync_add_and_fetch(&fifo->bytes_in_fifo, 0);

  while (now_bytes == 0) {
    if (fifo->done)
      goto fail; // FIFO will not be used by other side, and is empty

    // fifo_wrote() updates bytes_in_fifo and then sets the event,
    // so there's no chance for oversleeping. On the other hand, it's 
    // possible that the data was drained between the bytes_in_fifo
    // update and the event setting, leading to a false wakeup.
    // That's why we're in a while loop ( + other race conditions).
    
    info->slept = 1;

    if (WaitForSingleObject(fifo->read_event, INFINITE) != WAIT_OBJECT_0) {
      errorprint("fifo_request_drain: Failed waiting for event", GetLastError());
      goto fail;
    }
    now_bytes = __sync_add_and_fetch(&fifo->bytes_in_fifo, 0);
  }

  max_bytes = fifo->size - fifo->read_position;
  taken = (now_bytes < max_bytes) ? now_bytes : max_bytes;
  info->addr = fifo->baseaddr + fifo->read_position;

 fail:
  info->bytes = taken;
  info->position = fifo->read_position;

  return taken;
}

void fifo_drained(struct xillyfifo *fifo,
		  int req_bytes) {

  unsigned int now_bytes;

  if (req_bytes == 0)
    return;

  now_bytes = __sync_sub_and_fetch(&fifo->bytes_in_fifo, req_bytes);
  __sync_add_and_fetch(&fifo->read_total, req_bytes);
  
  fifo->read_position += req_bytes;

  if (fifo->read_position >= fifo->size)
    fifo->read_position -= fifo->size;

  if (!SetEvent(fifo->write_event))
    errorprint("fifo_drained: Failed to set write event", GetLastError());
}

int fifo_request_write(struct xillyfifo *fifo,
		       struct xillyinfo *info) {
  int taken = 0;
  unsigned int now_bytes, max_bytes;

  info->slept = 0;
  info->addr = NULL;

  now_bytes = __sync_add_and_fetch(&fifo->bytes_in_fifo, 0);

  if (fifo->done)
    goto fail; // No point filling an abandoned FIFO

  while (now_bytes >= (fifo->size - FIFO_BACKOFF)) {
    // fifo_drained() updates bytes_in_fifo and then sets the event,
    // so there's no chance for oversleeping. On the other hand, it's 
    // possible that the data was drained between the bytes_in_fifo
    // update and the event setting, leading to a false wakeup.
    // That's why we're in a while loop ( + other race conditions).

    info->slept = 1;

    if (WaitForSingleObject(fifo->write_event, INFINITE) != WAIT_OBJECT_0) {
      errorprint("fifo_request_write: Failed waiting for event", GetLastError());
      goto fail;
    }
  
    if (fifo->done)
      goto fail; // No point filling an abandoned FIFO

    now_bytes = __sync_add_and_fetch(&fifo->bytes_in_fifo, 0);
  }

  taken = fifo->size - (now_bytes + FIFO_BACKOFF);

  max_bytes = fifo->size - fifo->write_position;

  if (taken > ((int) max_bytes))
    taken = max_bytes;
  info->addr = fifo->baseaddr + fifo->write_position;

 fail:
  info->bytes = taken;
  info->position = fifo->write_position;

  return taken;
}

void fifo_wrote(struct xillyfifo *fifo,
		 int req_bytes) {

  unsigned int now_bytes;

  if (req_bytes == 0)
    return;

  now_bytes = __sync_add_and_fetch(&fifo->bytes_in_fifo, req_bytes);
  __sync_add_and_fetch(&fifo->write_total, req_bytes);
  
  fifo->write_position += req_bytes;
  
  if (fifo->write_position >= fifo->size)
    fifo->write_position -= fifo->size;
  
  if (!SetEvent(fifo->read_event))
    errorprint("fifo_wrote: Failed to set read event", GetLastError());
}

/*********************************************************************
 *                                                                   *
 *                 A P P L I C A T I O N   C O D E                   *
 *                                                                   *
 *********************************************************************/

// Read from FIFO, write to standard output

DWORD WINAPI write_thread(LPVOID arg)
{
  struct xillyfifo *fifo = arg;
  int do_bytes, written_bytes;
  struct xillyinfo info;
  unsigned char *buf;

  while (1) {
    do_bytes = fifo_request_drain(fifo, &info);

    if (do_bytes == 0)
      return 0;

    for (buf = info.addr; do_bytes > 0;
	 buf += written_bytes, do_bytes -= written_bytes) {

      written_bytes = _write(1, buf, do_bytes);

      if ((written_bytes < 0) && (errno != EINTR)) {
	perror("write() failed");
	return 0;
      }

      if (written_bytes == 0) {
	fprintf(stderr, "Reached write EOF (?!)\n");
	fifo_done(fifo);
	return 0;
      }

      if (written_bytes < 0) { // errno is EINTR
	written_bytes = 0;
	continue;
      }
      
      fifo_drained(fifo, written_bytes);
    }
  }
}

// Write to FIFO, read from standard output

DWORD WINAPI read_thread(LPVOID arg)
{
  struct xillyfifo *fifo = arg;
  int do_bytes, read_bytes;
  struct xillyinfo info;
  unsigned char *buf;

  while (1) {
    do_bytes = fifo_request_write(fifo, &info);

    if (do_bytes == 0)
      return 0;

    for (buf = info.addr; do_bytes > 0;
	 buf += read_bytes, do_bytes -= read_bytes) {

      read_bytes = _read(read_fd, buf, do_bytes);

      if ((read_bytes < 0) && (errno != EINTR)) {
	perror("read() failed");
	return 0;
      }

      if (read_bytes == 0) {
	// Reached EOF. Quit without complaining.
	fifo_done(fifo);
	return 0;
      }

      if (read_bytes < 0) { // errno is EINTR
	read_bytes = 0;
	continue;
      }
      
      fifo_wrote(fifo, read_bytes);
    }
  }
}


DWORD WINAPI status_thread(LPVOID arg) {
  struct xillyfifo *fifo = arg;
  
  while (fifo->done < 2)
    fprintf(stderr, "%9d bytes in FIFO, %12ld read, %12ld written\r",
	    __sync_add_and_fetch(&fifo->bytes_in_fifo, 0),
	    __sync_add_and_fetch(&fifo->read_total, 0),
	    __sync_add_and_fetch(&fifo->write_total, 0)	   
	    ); 
  return 0;
}

int __cdecl main(int argc, char *argv[]) {
  HANDLE tid[3];
  struct xillyfifo fifo;
  unsigned int fifo_size;

  if ((argc != 2) && (argc != 3)) {
    fprintf(stderr, "Usage: %s fifo_size [read-file]\n", argv[0]);
    exit(1);
  }

  fifo_size = atoi(argv[1]);

  if (fifo_size == 0) {
    fprintf(stderr, "Bad fifo_size argument %s\n", argv[1]);
    exit(1);
  }

  // If more than one FIFO is created, use the total memory needed instead
  // of fifo_size with SetProcessWorkingSetSize()

  if ((fifo_size > 20000) &&
      !SetProcessWorkingSetSize(GetCurrentProcess(),
				1024*1024 + fifo_size,
				2048*1024 + fifo_size))
    errorprint("Failed to enlarge unswappable RAM limit", GetLastError());
  

  if (fifo_init(&fifo, fifo_size)) {
    perror("Failed to init");
    exit(1);
  }

  if (argc > 2) {
    read_fd = _open(argv[2], O_RDONLY | _O_BINARY);
    
    if (read_fd < 0) {
      perror("Failed to open read file");
      exit(1);
    }
  } else {
    if (_setmode(0, _O_BINARY) < 0)
      fprintf(stderr, "Failed to set binary mode for standard input\n");  
  }

  if (_setmode(1, _O_BINARY) < 0)
    fprintf(stderr, "Failed to set binary mode for standard output\n");

  // default security, default stack size, default startup flags
  tid[0] = CreateThread(NULL, 0, read_thread, &fifo, 0, NULL);

  if (tid[0] == NULL) {
    errorprint("Failed to create thread", GetLastError());
    exit(1);
  }

  tid[1] = CreateThread(NULL, 0, write_thread, &fifo, 0, NULL);
  
  if (tid[1] == NULL) {
    errorprint("Failed to create thread", GetLastError());
    exit(1);
  }

  tid[2] = CreateThread(NULL, 0, status_thread, &fifo, 0, NULL);
  
  if (tid[2] == NULL) {
    errorprint("Failed to create thread", GetLastError());
    exit(1);
  }

  // Wait for threads to exit

  if (WaitForSingleObject(tid[0], INFINITE) != WAIT_OBJECT_0) 
    errorprint("Failed waiting for read_thread to terminate", GetLastError());

  if (WaitForSingleObject(tid[1], INFINITE) != WAIT_OBJECT_0) 
    errorprint("Failed waiting for write_thread to terminate", GetLastError());

  fifo.done = 2; // This is a hack for the status thread

  if (WaitForSingleObject(tid[2], INFINITE) != WAIT_OBJECT_0) 
    errorprint("Failed waiting for status_thread to terminate", GetLastError());

  fifo_destroy(&fifo);

  return 0;
}
