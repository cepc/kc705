#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/* memread.c -- Demonstrate read from a Xillybus dual port memory
   
This simple command-line application is given two arguments: The device file
to write to and the address (a decimal number between 0 and 31).

The application sends a seek command to the device file and reads the
character. These two operations create a read operation on the given address
on the FPGA's dual port RAM.

The use of allread() is an overkill when a single byte is read. For
longer reads it's the adequate way to do what most programmers expect:
Read as requested, or never return.

See http://www.xillybus.com/doc/ for usage examples an information.

*/
  
void allread(int fd, unsigned char *buf, int len);

int main(int argc, char *argv[]) {

  int fd;
  int address;
  unsigned char data;

  if (argc!=3) {
    fprintf(stderr, "Usage: %s devfile address\n", argv[0]);
    exit(1);
  }
  
  address = atoi(argv[2]);

  fd = _open(argv[1], O_RDONLY | _O_BINARY);
  
  if (fd < 0) {
    if (errno == ENODEV)
      fprintf(stderr, "(Maybe %s a write-only file?)\n", argv[1]);

    perror("Failed to open devfile");
    exit(1);
  }

  if (_lseek(fd, address, SEEK_SET) < 0) {
    perror("Failed to seek");
    exit(1);
  }

  allread(fd, &data, 1);

  printf("Read from address %d: %d\n", address, (int) data);

  return 0;
}

/* 
   Plain read() may not read all bytes requested in the buffer, so
   allread() loops until all data was indeed read, or exits in
   case of failure, except for EINTR. The way the EINTR condition is
   handled is the standard way of making sure the process can be suspended
   with CTRL-Z and then continue running properly.

   The function has no return value, because it always succeeds (or exits
   instead of returning).

   The function doesn't expect to reach EOF either.
*/


void allread(int fd, unsigned char *buf, int len) {
  int received = 0;
  int rc;

  while (received < len) {
    rc = _read(fd, buf + received, len - received);
	
    if ((rc < 0) && (errno == EINTR))
      continue;

    if (rc < 0) {
      perror("allread() failed to read");
      exit(1);
    }
	
    if (rc == 0) {
      fprintf(stderr, "Reached read EOF (?!)\n");
      exit(1);
    }
 
    received += rc;
  }
} 
