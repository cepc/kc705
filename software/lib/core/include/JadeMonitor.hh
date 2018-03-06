#ifndef JADE_JADEMONITOR_HH
#define JADE_JADEMONITOR_HH

#include "JadeSystem.hh"
#include "JadeDataFrame.hh"

class DLLEXPORT JadeMonitor{
 public:
  JadeMonitor(const std::string& options);
  virtual void Monitor(JadeDataFrameSP df);
 private:
  std::string m_options;
  size_t m_ev_print;
  size_t m_ev_n;
};

using JadeMonitorSP = std::shared_ptr<JadeMonitor>;

#endif
