%module(directors="1") pyJadePixAna
%include "std_string.i"
%{
#include "Decode.hh"
#include "wrapdecode2root.hh"
#include "wrappixelana.hh"
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

%include "Decode.hh"
%include "wrapdecode2root.hh"
%include "wrappixelana.hh"
