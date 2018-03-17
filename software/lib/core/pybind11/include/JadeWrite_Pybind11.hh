#ifndef JADE_JADEWRITE_PYBIND11_HH
#define JADE_JADEWRITE_PYBIND11_HH

#include "pybind11/pybind11.h"
#include "JadeWrite.hh"

class PyJadeWrite: public JadeWrite{
public:
  using JadeWrite::JadeWrite;
  static void InitPybind11(pybind11::module &m);  
};

#endif
