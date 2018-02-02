#include "JadeWrite_Pybind11.hh"

void PyJadeWrite::InitPybind11(pybind11::module &m){
  pybind11::class_<JadeWrite, PyJadeWrite, std::shared_ptr<JadeWrite>>
    classdef(m, "JadeWrite");
}
