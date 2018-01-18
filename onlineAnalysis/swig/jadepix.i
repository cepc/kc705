%module(directors="1") jadepix
%{
#include "JadepixDecoder.hh"
#include "JadepixFrame.hh"
#include "JadepixDigi.hh"
%}

%import "std_string.i"
%import "std_map.i"
%import "std_pair.i"
%import "std_vector.i"

%include "JadepixDecoder.hh"
%include "JadepixFrame.hh"
%include "JadepixDigi.hh"
