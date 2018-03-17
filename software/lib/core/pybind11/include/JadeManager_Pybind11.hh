#ifndef JADE_JADEMANAGER_PYBIND11_HH
#define JADE_JADEMANAGER_PYBIND11_HH

#include "pybind11/pybind11.h"
#include <pybind11/stl.h>
#include <pybind11/complex.h>
#include "JadeManager.hh"

class PyJadeManager: public JadeManager{
public:
  using JadeManager::JadeManager;
  static void InitPybind11(pybind11::module &m);  
};

#endif
