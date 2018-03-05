#ifndef JADE_JADEFILTER_HH
#define JADE_JADEFILTER_HH

#include "JadeSystem.hh"
#include "JadeDataFrame.hh"

class DLLEXPORT JadeFilter{
public:
  JadeFilter(const std::string &options);
  virtual ~JadeFilter();
  virtual JadeDataFrameSP Filter(JadeDataFrameSP df);
  
private:
  std::string m_options;  
};

using JadeFilterSP = std::shared_ptr<JadeFilter>;

#endif
