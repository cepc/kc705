#ifndef JADE_JADEFILTER_HH
#define JADE_JADEFILTER_HH

#include "JadeSystem.hh"
#include "JadeDataFrame.hh"

class JadeFilter{
public:
  JadeFilter(const std::string &options);
  virtual ~JadeFilter();
  virtual bool Filter(const JadeDataFrame &) const;
  
private:
  std::string m_options;

};



#endif
