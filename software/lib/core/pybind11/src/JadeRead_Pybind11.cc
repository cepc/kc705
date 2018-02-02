#include "JadeRead_Pybind11.hh"

void PyJadeRead::InitPybind11(pybind11::module &m){
  pybind11::class_<JadeRead, PyJadeRead, std::shared_ptr<JadeRead>>
    classdef(m, "JadeRead");
}
