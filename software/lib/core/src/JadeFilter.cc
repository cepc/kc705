#include "JadeFilter.hh"
#include "JadeUtils.hh"

using _base_c_ = JadeFilter;
using _index_c_ = JadeFilter;

template class DLLEXPORT JadeFactory<_base_c_>;
template DLLEXPORT
    std::unordered_map<std::type_index, typename JadeFactory<_base_c_>::UP (*)(const JadeOption&)>&
    JadeFactory<_base_c_>::Instance<const JadeOption&>();

namespace {
auto _loading_index_ = JadeUtils::SetTypeIndex(std::type_index(typeid(_index_c_)));
auto _loading_ = JadeFactory<_base_c_>::Register<_index_c_, const JadeOption&>(typeid(_index_c_));
}

using namespace std::chrono_literals;

JadeFilter::JadeFilter(const JadeOption& opt)
{
}

JadeFilter::~JadeFilter()
{
}

JadeFilterSP JadeFilter::Make(const std::string& name, const JadeOption& opt)
{
  try {
    std::type_index index = JadeUtils::GetTypeIndex(name);
    JadeFilterSP wrt = JadeFactory<JadeFilter>::MakeUnique<const JadeOption&>(index, opt);
    return wrt;
  } catch (...) {
    std::cout << "TODO: JadeFilter" << std::endl;
    return nullptr;
  }
}

JadeDataFrameSP JadeFilter::Filter(JadeDataFrameSP df)
{
  return df;
}

JadeOption JadeFilter::Post(const std::string& url, const JadeOption& opt)
{
  return JadePost::Post(url, opt);
}


