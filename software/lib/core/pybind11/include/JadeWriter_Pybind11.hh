#ifndef JADE_JADEWRITE_PYBIND11_HH
#define JADE_JADEWRITE_PYBIND11_HH

#include "pybind11/pybind11.h"
#include "JadeWriter.hh"

class PyJadeWriter: public JadeWriter{
public:
  using JadeWriter::JadeWriter;
  static void InitPybind11(pybind11::module &m);  
};

#endif
