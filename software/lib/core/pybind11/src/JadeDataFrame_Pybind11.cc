#include "JadeDataFrame_Pybind11.hh"

void PyJadeDataFrame::InitPybind11(pybind11::module &m){
  pybind11::class_<JadeDataFrame, PyJadeDataFrame, std::shared_ptr<JadeDataFrame>>
    classdef(m, "JadeDataFrame");
  classdef.def("Decode", &JadeDataFrame::Decode);
}

void PyJadeDataFrame::Decode(){
  PYBIND11_OVERLOAD(void, /* Return type */
		    JadeDataFrame,
		    Decode
		    );
}
