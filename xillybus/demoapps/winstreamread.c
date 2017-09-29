#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <windows.h>

/* winstreamread.c -- Demonstrate read from a Xillybus FIFO using Windows'
   API for reading
   
This simple command-line application is given one argument: The device
file to read from. The read data is sent to standard output.

This program has no advantage over the classic UNIX 'cat' command. It was
written merely to demonstrate the coding technique.

We don't use allread() here (see memread.c), because if less than the
desired number of bytes arrives, they should be handled immediately.

See http://www.xillybus.com/doc/ for usage examples an information.

*/

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


void allwrite(int fd, unsigned char *buf, int len);  

int main(int argc, char *argv[]) {
  HANDLE fh;
  DWORD rc;

  unsigned char buf[128];
  
  if (argc!=2) {
    fprintf(stderr, "Usage: %s devfile\n", argv[0]);
    exit(1);
  }
  
  fh = CreateFile(argv[1],                  // file to open
		  GENERIC_READ,             // open for reading
		  0,                        // do not share
		  NULL,                     // no security
		  OPEN_EXISTING,            // existing file only
		  FILE_ATTRIBUTE_NORMAL,
		  NULL);                  // no attr. template
  
  if (fh == INVALID_HANDLE_VALUE) {
    errorprint("Failed to open file", GetLastError());
    return 1;
  }

  while (1) {
    if (!ReadFile(fh,  buf, sizeof(buf), &rc, NULL)) {
      errorprint("ReadFile", GetLastError());
      return 1;
    } 

    if (rc == 0) {
      fprintf(stderr, "Reached read EOF.\n");
      return 0;
    }


    // Write all data to standard output = file descriptor 1
    // rc contains the number of bytes that were read.

    allwrite(1, buf, rc);
  }
}

/* 
   Plain write() may not write all bytes requested in the buffer, so
   allwrite() loops until all data was indeed written, or exits in
   case of failure, except for EINTR. The way the EINTR condition is
   handled is the standard way of making sure the process can be suspended
   with CTRL-Z and then continue running properly.

   The function has no return value, because it always succeeds (or exits
   instead of returning).

   The function doesn't expect to reach EOF either.
*/

void allwrite(int fd, unsigned char *buf, int len) {
  int sent = 0;
  int rc;

  while (sent < len) {
    rc = _write(fd, buf + sent, len - sent);
	
    if ((rc < 0) && (errno == EINTR))
      continue;

    if (rc < 0) {
      perror("allwrite() failed to write");
      exit(1);
    }
	
    if (rc == 0) {
      fprintf(stderr, "Reached write EOF (?!)\n");
      exit(1);
    }
 
    sent += rc;
  }
} 
