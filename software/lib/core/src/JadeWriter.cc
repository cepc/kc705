#include "JadeWriter.hh"
#include <ctime>

#include "JadeUtils.hh"

using _base_c_ = JadeWriter;
using _index_c_ = JadeWriter;

template class DLLEXPORT JadeFactory<_base_c_>;
template DLLEXPORT
std::unordered_map<std::type_index, typename JadeFactory<_base_c_>::UP (*)(const JadeOption&)>&
JadeFactory<_base_c_>::Instance<const JadeOption&>();

namespace{
  auto _loading_index_ = JadeUtils::SetTypeIndex(std::type_index(typeid(_index_c_)));
  auto _loading_ = JadeFactory<_base_c_>::Register<_index_c_, const JadeOption&>(typeid(_index_c_));
}

JadeWriter::JadeWriter(const JadeOption &opt){
}

JadeWriter::~JadeWriter(){
}

JadeWriterSP JadeWriter::Make(const std::string& name, const JadeOption& opt){  
  try{
    std::type_index index = JadeUtils::GetTypeIndex(name);
    JadeWriterSP wrt =  JadeFactory<JadeWriter>::MakeUnique<const JadeOption&>(index, opt);
    return wrt;
  }
  catch(...){
    std::cout<<"TODO"<<std::endl;
    return nullptr;
  }
}

void JadeWriter::Write(JadeDataFrameSP df){
}

JadeOption JadeWriter::Post(const std::string &url, const JadeOption &opt){
  return JadePost::Post(url, opt);
}

