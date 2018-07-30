#include "JadeWriter_Pybind11.hh"

void PyJadeWriter::InitPybind11(pybind11::module &m){
  pybind11::class_<JadeWriter, PyJadeWriter, std::shared_ptr<JadeWriter>>
    classdef(m, "JadeWriter");
}
