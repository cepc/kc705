%module(directors="1") daq2
%feature("director") EventListener;
%{
#include "daq2.h"
%}
%include "daq2.h"
%ignore DataTakingThread;