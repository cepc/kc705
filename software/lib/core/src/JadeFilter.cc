#include "JadeFilter.hh"
#include "JadeUtils.hh"

using _base_c_ = JadeFilter;
using _index_c_ = JadeFilter;

template class DLLEXPORT JadeFactory<_base_c_>;
template DLLEXPORT
std::unordered_map<std::type_index, typename JadeFactory<_base_c_>::UP (*)(const JadeOption&)>&
JadeFactory<_base_c_>::Instance<const JadeOption&>();

namespace{
  auto _loading_index_ = JadeUtils::SetTypeIndex(std::type_index(typeid(_index_c_)));
  auto _loading_ = JadeFactory<_base_c_>::Register<_index_c_, const JadeOption&>(typeid(_index_c_));
}

using namespace std::chrono_literals;

JadeFilter::JadeFilter(const JadeOption &opt)
  :m_opt(opt){
  
}

JadeFilterSP JadeFilter::Make(const std::string& name, const JadeOption& opt){  
  try{
    std::type_index index = JadeUtils::GetTypeIndex(name);
    JadeFilterSP wrt =  JadeFactory<JadeFilter>::MakeUnique<const JadeOption&>(index, opt);
    return wrt;
  }
  catch(...){
    std::cout<<"TODO"<<std::endl;
    return nullptr;
  }
}

JadeFilter::~JadeFilter(){
  
}

void JadeFilter::Reset(){
  
}

JadeDataFrameSP JadeFilter::Filter(JadeDataFrameSP df){
  return df;
}

JadeOption JadeFilter::Post(const std::string &url, const JadeOption &opt){    
  if(url == "/Reset"){
    Reset();
    return "{\"status\":true}";
  }

  static const std::string url_base_class("/JadePost/");
  if( ! url.compare(0, url_base_class.size(), url_base_class) ){
    return JadePost::Post(url.substr(url_base_class.size()-1), opt);
  }
  return JadePost::Post(url, opt);
}
