//#include "stdafx.h"
#include "pcie_diver.h"
/*int Add ( int a , int b )
{
      return ( a + b );
}*/

#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/* streamread.c -- Demonstrate read from a Xillybus FIFO.
   
This simple command-line application is given one argument: The device
file to read from. The read data is sent to standard output.

This program has no advantage over the classic UNIX 'cat' command. It was
written merely to demonstrate the coding technique.

We don't use allread() here (see memread.c), because if less than the
desired number of bytes arrives, they should be handled immediately.

See http://www.xillybus.com/doc/ for usage examples an information.

*/

//void allwrite(int fd, unsigned char *buf, int len);  

int pcie_diver(char* deciveid, char *serviceio[],unsigned char *data_output) {

  int fd, rc;	
  int i=0;int ni=0;
  unsigned char buf[400];
  int j=0;	
 
  int lengthl,lngth;
 
  int c=0;  int a=1;  int b=2;
  //data_output[0]=0;
  /* data length*/
  //  if(*argv[3]='longlength')
//	  lngth=512;
  //if(*argv[3]='smalllength')
	//  lngth=4;
 
  /*exe方式执行*/
/*  
  if (argc!=2) {
    fprintf(stderr, "Usage: %s devfile\n", argv[0]);
    exit(1);
  }
    fd = _open(argv[1], O_RDONLY | _O_BINARY);
  */
  
 /*memoy读取长度可调*/
  /*：：main(int argc,char *argv[],int length)*/
/*  
  if (argc!=3) {
    fprintf(stderr, "Usage: %s devfile\n", argv[0]);
    exit(1);
  }
    fd = _open(argv[1], O_RDONLY | _O_BINARY);
  */
  if (*deciveid !='0x0a') {
    fprintf(stderr, "Usage: %s devfile\n", serviceio);
    exit(1);
  }
#define _DATACHANNELCHOSE 1
#ifdef _DATACHANNELCHOSE
   fd = _open(serviceio[0], O_RDONLY | _O_BINARY);
   //fd = _open("\\\\.\\xillybus_read_32", O_RDONLY | _O_BINARY);
#else
    
	fd = _open("\\\\.\\xillybus_read_32", O_RDONLY | _O_BINARY);
#endif	  
/*
  #pragma message("消息文本")
#pragma warning(error:164)//把164号警告信息作为一个错误。

#define _X86 67
#ifdef _X86
#pragma message("_X86 macro activated!")
#endif
  */
  //ubbish code
/**/
  printf("argc=%s\n",deciveid);
  
 if (fd < 0) {
    if (errno == ENODEV)
      fprintf(stderr, "(Maybe %s a write-only file?)\n", serviceio);
	  fprintf(stderr, "(Maybe %s a write-only file?)\n", "xillybus_read_32");
	  perror("Failed to open devfile");
      exit(1);
  }
 // printf("dargv[1]=%d\n",(int)argv[1]);
 // printf("cargv[2]=%s\n",argv[2]);
//  printf("cargv[2]=%d\n",(int)argv[2]);

  //printf("cargv[1]=%s\n",argv[1]);
  //printf("cargv[0]=%s\n",argv[0]);

  //while (1) {
  //  for(ni=0;ni<lngth;ni++)
	{rc = _read(fd, buf, sizeof(buf));
    
    if ((rc < 0) && (errno == EINTR))
      //continue; 
	  exit(1);
    
    if (rc < 0) {
      perror("allread() failed to read");
      exit(1);
    }
    
    if (rc == 0) {
      fprintf(stderr, "Reached read EOF.\n");
      exit(0);
    }
 
    // Write all data to standard output = file descriptor 1
    // rc contains the number of bytes that were read.

    //allwrite(1, buf, rc);
	//length设置

//	lengthl=(int)argv[1]*128;
	printf("rc=%d: ,the count addess is %x:\n",rc,lengthl);
	printf("\n ");

	for(j=0;j<rc/8;j++)
	{	
		
		printf("addr %x0: ",j);
		for(i=0;i<8;i++)
	{
		printf("%x ",buf[i]);
	//	data_output[i]=buf[i];
		data_output[i]=i;
	}
	printf("\n ");
		}
		
  }
	  		//return buf;
		system("pause");
}
