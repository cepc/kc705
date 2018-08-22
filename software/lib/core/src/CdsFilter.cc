#include "JadeFilter.hh"
#include "JadeUtils.hh"

//++++++++++++++++++++++++++++++++++++++++++++++++++++
//CdsFilter.hh
class CdsFilter : public JadeFilter {
  public:
  CdsFilter(const JadeOption& opt);
  ~CdsFilter() override{};
  JadeOption Post(const std::string& url, const JadeOption& opt) override;
  JadeDataFrameSP Filter(JadeDataFrameSP df) override;
  void Reset() override;

  private:
  JadeDataFrameSP m_df_last;
};

//+++++++++++++++++++++++++++++++++++++++++
//CdsFilter.cc
namespace {
auto _cds_index_ = JadeUtils::SetTypeIndex(std::type_index(typeid(CdsFilter)));
auto _cds_ = JadeFactory<JadeFilter>::Register<CdsFilter, const JadeOption&>(typeid(CdsFilter));
}

CdsFilter::CdsFilter(const JadeOption& opt)
    : JadeFilter(opt)
{
}

JadeDataFrameSP CdsFilter::Filter(JadeDataFrameSP df)
{
  //std::cout<<"filtering"<<std::endl;
  //if (!m_df_last) {
  //  m_df_last = df;
  //  std::cout << "first data arrives" << std::endl;
  //  return nullptr;
  //}

  //if(m_df_last->GetFrameCount() +1 != df->GetFrameCount()){
  auto trigger_serial_order = df->GetTriggerSerialOrder();
  if (trigger_serial_order == 0) {
    m_df_last = df;
    //std::cout << "FrameCount mismatch, skipped" << std::endl;
    return nullptr;
  }

  //TODO, reuse the memory of m_df_last, since it will be delelted soon
  JadeDataFrameSP df_cds = JadeDataFrame::CdsAndReturnNewObject(*m_df_last, *df);
  m_df_last = df;
  return df_cds;
}

void CdsFilter::Reset()
{
  m_df_last;
}

JadeOption CdsFilter::Post(const std::string& url, const JadeOption& opt)
{
  static const std::string url_base_class("/JadeFilter/");
  if (!url.compare(0, url_base_class.size(), url_base_class)) {
    return JadeFilter::Post(url.substr(url_base_class.size() - 1), opt);
  }
  return JadePost::Post(url, opt);
}
