#include "JadeRegCtrl_Pybind11.hh"

void PyJadeRegCtrl::InitPybind11(pybind11::module &m){
  pybind11::class_<JadeRegCtrl, PyJadeRegCtrl,
		   std::shared_ptr<JadeRegCtrl>>
    classdef(m, "JadeRegCtrl");
  classdef.def(pybind11::init<const std::string&, const std::map<std::string, std::pair<uint16_t, uint8_t>>& >());
  classdef.def(pybind11::init<const std::string&>());
  classdef.def("WriteByte", &JadeRegCtrl::WriteByte);
  classdef.def("ReadByte", &JadeRegCtrl::ReadByte);
  classdef.def("Command", pybind11::overload_cast<const std::string&>(&JadeRegCtrl::Command));
  classdef.def("Command", pybind11::overload_cast<const std::string&, uint8_t>(&JadeRegCtrl::Command));
}
