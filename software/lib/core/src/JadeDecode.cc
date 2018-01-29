#include "JadeDecode.hh"

JadeDecode::JadeDecode(const std::string &options)
  :m_options(options){

}


JadeDecode::~JadeDecode(){

}

JadeDataFrame JadeDecode::Decode(JadeDataFrame &&df) const{
  return df;
}
