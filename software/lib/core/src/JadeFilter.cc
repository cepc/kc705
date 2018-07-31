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

JadeFilter::JadeFilter(const JadeOption &opt){
}

JadeFilter::~JadeFilter(){  
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

JadeDataFrameSP JadeFilter::Filter(JadeDataFrameSP df){
  return df;
}

JadeOption JadeFilter::Post(const std::string &url, const JadeOption &opt){
  return JadePost::Post(url, opt);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++
//TestFilter.hh
class TestFilter: public JadeFilter{
 public:
  TestFilter(const JadeOption &opt);
  ~TestFilter() override {};
  JadeOption Post(const std::string &url, const JadeOption &opt) override;
  JadeDataFrameSP Filter(JadeDataFrameSP df) override;
};

//+++++++++++++++++++++++++++++++++++++++++
//TestFilter.cc
namespace{
  auto _test_index_ = JadeUtils::SetTypeIndex(std::type_index(typeid(TestFilter)));
  auto _test_ = JadeFactory<JadeFilter>::Register<TestFilter, const JadeOption&>(typeid(TestFilter));
}

TestFilter::TestFilter(const JadeOption &opt)
  :JadeFilter(opt){
}

JadeDataFrameSP TestFilter::Filter(JadeDataFrameSP df){
  return df;
}

JadeOption TestFilter::Post(const std::string &url, const JadeOption &opt){
  if(url == "/testurl"){
    std::cout<<"TestFilter: INFO, url<"<<url<<"> opt<"<<opt.DumpString()<<">\n"<<std::endl;
    return "{\"status\":true}";
  }

  static const std::string url_base_class("/JadeFilter/");
  if( ! url.compare(0, url_base_class.size(), url_base_class) ){
    return JadeFilter::Post(url.substr(url_base_class.size()-1), opt);
  }
  return JadePost::Post(url, opt);
}
