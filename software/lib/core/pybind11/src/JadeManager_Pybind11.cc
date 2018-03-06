#include "JadeManager_Pybind11.hh"

void PyJadeManager::InitPybind11(pybind11::module &m){
  pybind11::class_<JadeManager, PyJadeManager, std::shared_ptr<JadeManager>>
    classdef(m, "JadeManager");
  classdef.def(pybind11::init<>());
  classdef.def("StartDataTaking", &JadeManager::StartDataTaking);
  classdef.def("StopDataTaking", &JadeManager::StopDataTaking);
}
