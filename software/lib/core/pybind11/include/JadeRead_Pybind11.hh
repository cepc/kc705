#ifndef JADE_JADEREAD_PYBIND11_HH
#define JADE_JADEREAD_PYBIND11_HH

#include "pybind11/pybind11.h"
#include "JadeRead.hh"

class PyJadeRead: public JadeRead{
public:
  static void InitPybind11(pybind11::module &m);  
};

#endif
