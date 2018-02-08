#ifndef JADE_JADEFILTER_HH
#define JADE_JADEFILTER_HH

#include "JadeSystem.hh"
#include "JadeDataFrame.hh"

#include <string>
#include <chrono>
#include <mutex>
#include <queue>

class DLLEXPORT JadeFilter{
public:
  JadeFilter(const std::string &options);
  virtual ~JadeFilter();
  virtual JadeDataFrameUP Filter(JadeDataFrameUP &&df) const;
  
private:
  std::string m_options;
  size_t m_ev_print;
  size_t m_ev_n;
  
  
};



#endif
