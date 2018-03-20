#ifndef JADE_JADEDATAFRAME_PYBIND11_HH
#define JADE_JADEDATAFRAME_PYBIND11_HH

#include "pybind11/pybind11.h"
#include "JadeDataFrame.hh"

class PyJadeDataFrame : public JadeDataFrame{
public:
  using JadeDataFrame::JadeDataFrame;
  static void InitPybind11(pybind11::module &m);

  void Decode() override;
};

#endif
