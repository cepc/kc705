#include "JadeDataFrame_Pybind11.hh"

#include <sstream>

void PyJadeDataFrame::InitPybind11(pybind11::module &m){
  pybind11::class_<JadeDataFrame, PyJadeDataFrame, std::shared_ptr<JadeDataFrame>>
    classdef(m, "JadeDataFrame");
  classdef.def("__repr__", [](const JadeDataFrame &df){
      std::ostringstream oss;
      df.Print(oss, 0);
      return oss.str();
    });
  
  classdef.def("Decode", &JadeDataFrame::Decode);
  classdef.def("RawDataString", &JadeDataFrame::RawDataString, pybind11::return_value_policy::reference_internal);
  classdef.def("DescriptionString", &JadeDataFrame::DescriptionString, pybind11::return_value_policy::reference_internal);
  classdef.def("TimeStamp", &JadeDataFrame::TimeStamp, pybind11::return_value_policy::reference_internal);
  classdef.def("GetFrameCount", &JadeDataFrame::GetFrameCount, pybind11::return_value_policy::copy);
  classdef.def("GetHitValue", &JadeDataFrame::GetHitValue, pybind11::return_value_policy::copy);
  classdef.def("IsInMatrix", &JadeDataFrame::IsInMatrix, pybind11::return_value_policy::copy);
  classdef.def("Print", &JadeDataFrame::Print);  
}

void PyJadeDataFrame::Decode(){
  PYBIND11_OVERLOAD(void, /* Return type */
		    JadeDataFrame,
		    Decode
		    );
}
