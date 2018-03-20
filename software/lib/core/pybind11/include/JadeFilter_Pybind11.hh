#ifndef JADE_JADEFILTER_PYBIND11_HH
#define JADE_JADEFILTER_PYBIND11_HH

#include "pybind11/pybind11.h"
#include "JadeFilter.hh"

class PyJadeFilter: public JadeFilter{
public:
  using JadeFilter::JadeFilter;
  static void InitPybind11(pybind11::module &m);  
};

#endif
