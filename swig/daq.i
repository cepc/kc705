%module(directors="1") daq
%include "std_string.i"
%feature("director") EventListener;
%{
#include "daq.h"
%}

// This is copied from cstring.i,
// however I changed string -> bytes
%define %cbytes_chunk_output(TYPEMAP,SIZE)
%typemap(in,numinputs=0) TYPEMAP(char temp[SIZE]) {
   $1 = ($1_ltype) temp;
}
%typemap(argout,fragment="t_output_helper") TYPEMAP {
   PyObject *o = PyBytes_FromStringAndSize($1,SIZE);
   $result = t_output_helper($result,o);
}
%enddef

%cbytes_chunk_output(char *data, 1928);
void DataTaker::getRecentEvent(char *data);

%ignore DataTakingThread;

%include "daq.h"
