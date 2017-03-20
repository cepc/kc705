#include <iostream>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <io.h>//for _open & _read
#include <errno.h>
#include <fcntl.h>// for _O_BINARY etc.

#define filenumber 20
#define bytesperpack 2000
#define packperfile 100

using namespace std;

int main()
{		
	int fd = 0;//open status
	int rc=0;//return status
	int rctotal = 0;//total count for once loop
	unsigned int filecount = 0;//loop count for files
	char ddbuffer[1000] = { 0 };//buffer to store source address as string
	int countforpack;//loop count for packs
	char FileName[10];//might need to be changed
	unsigned char buf[bytesperpack*2];//to store one pack data;need to confirm every data >0;twice larger in case last single read returns more bytes than needed

	LARGE_INTEGER m_nFreq;
	LARGE_INTEGER m_nBeginTime;
	LARGE_INTEGER nEndTime;
	QueryPerformanceFrequency(&m_nFreq);
	QueryPerformanceCounter(&m_nBeginTime);

	sprintf(ddbuffer, "//./xillybus_read_32");
	//sprintf(ddbuffer, "source");
	fd = _open(ddbuffer, O_RDONLY | _O_BINARY);//for open source

	if (fd < 0)
	{
		if (errno == ENODEV)
		{
			printf("Maybe a write-only file?\n" );//what is that?
		}
		perror("Failed to open devfile");
		exit(1);
	}	

	for ( filecount = 0; filecount < filenumber; filecount++)
	{
		sprintf(FileName, "%1d.txt", filecount);//not define at the beginning, should be defined
		FILE *out = fopen(FileName, "wb");//for open destination				

		for (countforpack = 0; countforpack < packperfile; countforpack++) 
		{
			rc = 0;
			rctotal = 0;
			memset(buf, 0, bytesperpack*2);
			while (rctotal < bytesperpack)
			{
				rc = _read(fd, buf + rctotal, bytesperpack - rctotal);
				//loop until read buf number data from fd to buf
				//if ((rc < 0) && (errno == EINTR))
					//continue;
				if (rc < 0) 
				{
					perror("allread() failed to read");
					exit(1);
				}
				if (rc == 0)
				{
					cout << "Reached read EOF"<< endl;
					exit(1);//if reached eof, drop last pack					
				}
				rctotal = rctotal + rc;
			}
			if (rctotal== bytesperpack)
			{
				if (buf[0] != 0xF0)
				{
					//printf("wrong head at pack %d\ for file %d\n", countforpack,filecount);
				}
				if (buf[rctotal - 1] != 0xAA)
				{
					//printf("wrong tail at pack %d for file %d\n", countforpack,filecount);
				}
			if(rctotal!=bytesperpack)
			{
				cout << "not enough data!" << endl;
				cout << "rctotal = " << rctotal << endl;
			}				
				//for all status above, just warning, still keep data
				fwrite(buf, rctotal, 1, out);//rc might not the same as bytesperpack
			}
	}

	fclose(out);
	}
	
	cout << "finish!" << endl;
	
	QueryPerformanceCounter(&nEndTime);
	cout << (double)(nEndTime.QuadPart - m_nBeginTime.QuadPart) * 1000 / m_nFreq.QuadPart << endl;

	return 0;
}
	

