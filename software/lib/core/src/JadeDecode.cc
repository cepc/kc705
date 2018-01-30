#include "JadeDecode.hh"

JadeDecode::JadeDecode(const std::string &options)
  :m_options(options){

}


JadeDecode::~JadeDecode(){

}

JadeDataFrameUP JadeDecode::Decode(JadeDataFrameUP &&df) const{
  return std::move(df);
}
