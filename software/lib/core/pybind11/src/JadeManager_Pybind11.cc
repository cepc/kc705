#include "JadeManager_Pybind11.hh"

void PyJadeManager::InitPybind11(pybind11::module &m){
  pybind11::class_<JadeManager, PyJadeManager, std::shared_ptr<JadeManager>>
    classdef(m, "JadeManager");
  classdef.def(pybind11::init<>());
  classdef.def("Start", pybind11::overload_cast<const std::string&, const std::string&>(&JadeManager::Start));
  classdef.def("Stop", &JadeManager::Stop);
}
