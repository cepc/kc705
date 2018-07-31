#ifndef JADE_JADEFILTER_HH
#define JADE_JADEFILTER_HH

#include "JadeSystem.hh"
#include "JadeFactory.hh"
#include "JadeOption.hh"
#include "JadePost.hh"
#include "JadeDataFrame.hh"

class JadeFilter;
using JadeFilterSP = JadeFactory<JadeFilter>::SP;
using JadeFilterUP = JadeFactory<JadeFilter>::UP;

#ifndef JADE_DLL_EXPORT
extern template class DLLEXPORT JadeFactory<JadeFilter>;
extern template DLLEXPORT
std::unordered_map<std::type_index, typename JadeFactory<JadeFilter>::UP (*)(const JadeOption&)>&
JadeFactory<JadeFilter>::Instance<const JadeOption&>();
#endif


class DLLEXPORT JadeFilter: public JadePost{
public:
  JadeFilter(const JadeOption &opt);
  ~JadeFilter() override;
  static JadeFilterSP Make(const std::string& name, const JadeOption& opt);
  JadeOption Post(const std::string &url, const JadeOption &opt) override;

  //reset inner state if it's not a stateless filter, do it before start a new datataking.
  virtual void Reset() {};
  //return nullptr if the data can not pass the filter
  virtual JadeDataFrameSP Filter(JadeDataFrameSP df);
};

#endif
