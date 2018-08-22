#include "JadeFilter.hh"
#include "JadeUtils.hh"

//++++++++++++++++++++++++++++++++++++++++++++++++++++
//TestFilter.hh
class TestFilter : public JadeFilter {
  public:
  TestFilter(const JadeOption& opt);
  ~TestFilter() override{};
  JadeOption Post(const std::string& url, const JadeOption& opt) override;
  JadeDataFrameSP Filter(JadeDataFrameSP df) override;
  void Reset() override;
};

//+++++++++++++++++++++++++++++++++++++++++
//TestFilter.cc
namespace {
auto _test_index_ = JadeUtils::SetTypeIndex(std::type_index(typeid(TestFilter)));
auto _test_ = JadeFactory<JadeFilter>::Register<TestFilter, const JadeOption&>(typeid(TestFilter));
}

TestFilter::TestFilter(const JadeOption& opt)
    : JadeFilter(opt)
{
}

JadeDataFrameSP TestFilter::Filter(JadeDataFrameSP df)
{
  return df;
}

void TestFilter::Reset()
{
}

JadeOption TestFilter::Post(const std::string& url, const JadeOption& opt)
{
  static const std::string url_base_class("/JadeFilter/");
  if (!url.compare(0, url_base_class.size(), url_base_class)) {
    return JadeFilter::Post(url.substr(url_base_class.size() - 1), opt);
  }
  return JadePost::Post(url, opt);
}
