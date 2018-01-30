#include "JadeDataFrame.hh"

JadeDataFrame::JadeDataFrame(std::string &&data)
  :m_data_raw(std::move(data)), m_is_decoded(false),
   m_offset_x(0), m_offset_y(0),
   m_n_x(0), m_n_y(0){
  
}

JadeDataFrame::JadeDataFrame(const std::string &data)
  :m_data_raw(data), m_is_decoded(false),
   m_offset_x(0), m_offset_y(0),
   m_n_x(0), m_n_y(0){
}

JadeDataFrame::JadeDataFrame(size_t nraw)
  :m_is_decoded(false), m_offset_x(0), m_offset_y(0),
   m_n_x(0), m_n_y(0){
  m_data_raw.resize(nraw);
}

JadeDataFrame::~JadeDataFrame(){
  
}

std::string& JadeDataFrame::RawDataString(){
  return m_data_raw;
}

void JadeDataFrame::Decode(){
  //TODO: decode raw data to hit
  m_is_decoded = true;
  return;
}

bool JadeDataFrame::IsInMatrix(size_t x, size_t y) const{
  if(x-m_offset_x< m_n_x && y-m_offset_y<m_n_y &&
     x>=m_offset_x && y>=m_offset_y)
    return true;
  else
    return false;
}

uint16_t JadeDataFrame::GetHitValue(size_t x, size_t y) const{
  size_t pos = (x-m_offset_x) + m_n_x*(y-m_offset_y);
  uint16_t val = m_data.at(pos);
  return val;
}
