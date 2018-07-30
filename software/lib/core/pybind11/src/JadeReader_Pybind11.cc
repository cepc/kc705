#include "JadeReader_Pybind11.hh"

void PyJadeReader::InitPybind11(pybind11::module &m){
  pybind11::class_<JadeReader, PyJadeReader, std::shared_ptr<JadeReader>>
    classdef(m, "JadeReader");
}
