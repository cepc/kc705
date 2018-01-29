#include "JadeDataFrame.hh"

JadeDataFrame::JadeDataFrame(std::string &&data)
  :m_data_raw(std::move(data)){

}

JadeDataFrame::JadeDataFrame(const std::string &data)
  :m_data_raw(data){

}

JadeDataFrame::~JadeDataFrame(){

}
