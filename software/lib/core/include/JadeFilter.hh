#ifndef JADE_JADEFILTER_HH
#define JADE_JADEFILTER_HH

#include "JadeSystem.hh"
#include "JadeOption.hh"
#include "JadeDataFrame.hh"

class DLLEXPORT JadeFilter{
public:
  JadeFilter(const JadeOption &opt);
  virtual ~JadeFilter();
  virtual void Reset(){};
  virtual JadeDataFrameSP Filter(JadeDataFrameSP df);
private:
  JadeOption m_opt;
};

using JadeFilterSP = std::shared_ptr<JadeFilter>;

#endif
