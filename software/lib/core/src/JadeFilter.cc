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
  auto _loading_ = JadeFactory<_base_c_>::Register<_base_c_, const JadeOption&>(typeid(_index_c_));
}


using namespace std::chrono_literals;

JadeFilter::JadeFilter(const JadeOption &opt)
  :m_opt(opt){
  
}

JadeFilter::~JadeFilter(){
  
}


void JadeFilter::Reset(){
  
}

JadeDataFrameSP JadeFilter::Filter(JadeDataFrameSP df){
  return df;
}
