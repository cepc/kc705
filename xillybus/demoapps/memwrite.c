#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/* memwrite.c -- Demonstrate write to a Xillybus dual port memory
   
This simple command-line application is given three arguments: The 
device file to write to, the address (a decimal number between 0 and 31) 
and data to be written (a decimal number between 0 and 255).

The application sends a seek command to the device file and writes the
character. These two operations create a write operation on the given address
on the FPGA's dual port RAM.

The use of allwrite() is an overkill when a single byte is written. For
longer writes it's the adequate way to do what most programmers expect:
Write all data, or never return.

See http://www.xillybus.com/doc/ for usage examples an information.

*/

void allwrite(int fd, unsigned char *buf, int len);

int main(int argc, char *argv[]) {

  int fd;
  int address;
  unsigned char data;

  if (argc!=4) {
    fprintf(stderr, "Usage: %s devfile address data\n", argv[0]);
    exit(1);
  }
  
  address = atoi(argv[2]);
  data = (unsigned char) atoi(argv[3]);

  fd = _open(argv[1], O_WRONLY | _O_BINARY);
  
  if (fd < 0) {
    if (errno == ENODEV)
      fprintf(stderr, "(Maybe %s a read-only file?)\n", argv[1]);

    perror("Failed to open devfile");
    exit(1);
  }

  if (_lseek(fd, address, SEEK_SET) < 0) {
    perror("Failed to seek");
    exit(1);
  }

  allwrite(fd, &data, 1);

  return 0;
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
