#ifndef _MYCODE_H_
#define _MYCODE_H_
#ifdef DLLDEMO1_EXPORTS
#define EXPORTS_DEMO _declspec( dllexport )
#else
#define EXPORTS_DEMO _declspec(dllimport)
#endif
extern "C" EXPORTS_DEMO int pcie_diver (char* deciveid, char *serviceio[],unsigned char *data_output);
#endif