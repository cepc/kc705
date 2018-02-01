#ifndef JADE_JADEMANAGER_PYBIND11_HH
#define JADE_JADEMANAGER_PYBIND11_HH

#include "pybind11/pybind11.h"
#include "JadeManager.hh"

class PyJadeManager: public JadeManager{
public:
  static void InitPybind11(pybind11::module &m);  
};

#endif
