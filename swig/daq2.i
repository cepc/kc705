%module(directors="1") daq2
%feature("director") EventListener;
%{
#include "daq2.h"
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

%cbytes_chunk_output(char *data, 98);
void DataTaker::getRecentEvent(char *data);

%ignore DataTakingThread;

%include "daq2.h"
