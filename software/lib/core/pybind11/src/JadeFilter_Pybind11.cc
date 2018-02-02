#include "JadeFilter_Pybind11.hh"

void PyJadeFilter::InitPybind11(pybind11::module &m){
  pybind11::class_<JadeFilter, PyJadeFilter, std::shared_ptr<JadeFilter>>
    classdef(m, "JadeFilter");
}

