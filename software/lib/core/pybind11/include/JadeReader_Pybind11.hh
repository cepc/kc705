#ifndef JADE_JADEREAD_PYBIND11_HH
#define JADE_JADEREAD_PYBIND11_HH

#include "pybind11/pybind11.h"
#include "JadeReader.hh"

class PyJadeReader: public JadeReader{
public:
  using JadeReader::JadeReader;
  static void InitPybind11(pybind11::module &m);  
};

#endif
