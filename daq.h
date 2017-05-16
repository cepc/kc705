//example.h
#include <iostream>
#include <iostream>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <io.h>//for _open & _read
#include <errno.h>
#include <fcntl.h>// for _O_BINARY etc.

using namespace std;
class Daq{
public:
void getdata();
};