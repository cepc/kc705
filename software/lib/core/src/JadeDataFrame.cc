#include "JadeDataFrame.hh"

#ifdef _WIN32
#pragma comment(lib, "Ws2_32.lib")
#endif

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

std::string& JadeDataFrame::DescriptionString(){
  return m_description;
}

void JadeDataFrame::Decode(){
  m_is_decoded = true;
  if(m_data_raw.size() != 1928){
    std::cerr<<"JadeDataFrame: unable to decode\n";
    throw;
  }
  m_n_x = 16;
  m_n_y = 48;
  m_data.clear();
  m_data.reserve(m_n_x*m_n_y);
  const char *p_raw = m_data_raw.data();
  m_description = m_data_raw.substr(0, 4);
  m_frame_n = be16toh(*reinterpret_cast<const uint16_t*>(p_raw+2));
  size_t p_offset = 4;
  for(size_t yn=0; yn<m_n_y; yn++){
    //Y head 4 bytes
    p_offset += 4;
    for(size_t xn=0; xn<m_n_x; yn++){
      uint16_t val=be16toh(*reinterpret_cast<const uint16_t*>
			   (p_raw+p_offset));
      m_data.push_back(val);
      p_offset +=2;
    }
    //Y tail 4 bytes
    p_offset += 4;
  }
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

void JadeDataFrame::Print(std::ostream & os, size_t offset) const{
  
}
