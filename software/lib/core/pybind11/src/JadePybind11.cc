#include <pybind11/pybind11.h>

#include "JadeDataFrame_Pybind11.hh"
#include "JadeManager_Pybind11.hh"
#include "JadeRegCtrl_Pybind11.hh"
#include "JadeRead_Pybind11.hh"
#include "JadeFilter_Pybind11.hh"
#include "JadeWrite_Pybind11.hh"

int add(int i, int j) {
    return i + j;
}

PYBIND11_MODULE(pyjade, m){
  m.doc() = "JADE DAQ library";
  m.def("add", &add, "A function which adds two numbers");
  PyJadeDataFrame::InitPybind11(m);
  PyJadeManager::InitPybind11(m);
  PyJadeRegCtrl::InitPybind11(m);
  PyJadeWrite::InitPybind11(m);
  PyJadeFilter::InitPybind11(m);
  PyJadeRead::InitPybind11(m);
}
