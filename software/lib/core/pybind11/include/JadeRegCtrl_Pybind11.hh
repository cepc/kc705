#ifndef JADE_JADEREGCTRL_PYBIND11_HH
#define JADE_JADEREGCTRL_PYBIND11_HH

#include "pybind11/pybind11.h"
#include "JadeRegCtrl.hh"

class PyJadeRegCtrl: public JadeRegCtrl{
public:
  static void InitPybind11(pybind11::module &m);  
};

#endif
