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
  void Reset() override;
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

void TestFilter::Reset(){

}

JadeOption TestFilter::Post(const std::string &url, const JadeOption &opt){
  static const std::string url_base_class("/JadeFilter/");
  if( ! url.compare(0, url_base_class.size(), url_base_class) ){
    return JadeFilter::Post(url.substr(url_base_class.size()-1), opt);
  }
  return JadePost::Post(url, opt);
}


//++++++++++++++++++++++++++++++++++++++++++++++++++++
//CdsFilter.hh
class CdsFilter: public JadeFilter{
 public:
  CdsFilter(const JadeOption &opt);
  ~CdsFilter() override {};
  JadeOption Post(const std::string &url, const JadeOption &opt) override;
  JadeDataFrameSP Filter(JadeDataFrameSP df) override;
  void Reset() override;

private:
  JadeDataFrameSP m_df_last;
};

//+++++++++++++++++++++++++++++++++++++++++
//CdsFilter.cc
namespace{
  auto _cds_index_ = JadeUtils::SetTypeIndex(std::type_index(typeid(CdsFilter)));
  auto _cds_ = JadeFactory<JadeFilter>::Register<TestFilter, const JadeOption&>(typeid(CdsFilter));
}

CdsFilter::CdsFilter(const JadeOption &opt)
  :JadeFilter(opt){
}

JadeDataFrameSP CdsFilter::Filter(JadeDataFrameSP df){
  if(m_df_last) {
    m_df_last = df;
    std::cout<<"first data arrives"<<std::endl;
    return nullptr;
  }

  if(m_df_last->GetFrameCount() +1 != df->GetFrameCount()){
    m_df_last = df;
    std::cout<<"FrameCount mismatch, skipped"<<std::endl;
    return nullptr;
  }

  //TODO, reuse the memory of m_df_last, since it will be delelted soon
  JadeDataFrameSP df_cds = JadeDataFrame::CdsAndReturnNewObject(*m_df_last, *df);
  m_df_last = df;
  return df_cds;
}

void CdsFilter::Reset(){
  m_df_last;
}

JadeOption CdsFilter::Post(const std::string &url, const JadeOption &opt){
  static const std::string url_base_class("/JadeFilter/");
  if( ! url.compare(0, url_base_class.size(), url_base_class) ){
    return JadeFilter::Post(url.substr(url_base_class.size()-1), opt);
  }
  return JadePost::Post(url, opt);
}
