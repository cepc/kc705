#include "JadeRegCtrl_Pybind11.hh"

void PyJadeRegCtrl::InitPybind11(pybind11::module &m){
  pybind11::class_<JadeRegCtrl, PyJadeRegCtrl, std::shared_ptr<JadeRegCtrl>>
    classdef(m, "JadeRegCtrl");
}
