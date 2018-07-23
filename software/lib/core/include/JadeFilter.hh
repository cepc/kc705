#ifndef JADE_JADEFILTER_HH
#define JADE_JADEFILTER_HH

#include "JadeDataFrame.hh"
#include "JadeOption.hh"
#include "JadeSystem.hh"

class DLLEXPORT JadeFilter {
  public:
  JadeFilter(const JadeOption& opt);
  virtual ~JadeFilter();
  virtual void Reset();
  virtual JadeDataFrameSP Filter(JadeDataFrameSP df);

  private:
  JadeOption m_opt;
};

using JadeFilterSP = std::shared_ptr<JadeFilter>;

#endif
