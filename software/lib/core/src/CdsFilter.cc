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

JadeDataFrameSP CdsFilter::Filter(JadeDataFrameSP df){
  if(m_df_last){
    df->GetTriggerN() != m_df_last->GetTriggerN();
    if(df->GetExtension()!= 0){
      std::cerr<<"CdsFilter: in the first dataframe of new triiger number, the extension word is not zero\n";
      throw;
    }
    m_df_last = df;
    return nullptr;
  }
  
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
