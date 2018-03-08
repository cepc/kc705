#ifndef JADE_JADEMONITOR_HH
#define JADE_JADEMONITOR_HH

#include "JadeSystem.hh"
#include "JadeDataFrame.hh"
#include "JadeOption.hh"

class DLLEXPORT JadeMonitor{
 public:
  JadeMonitor(const JadeOption& opt);
  virtual ~JadeMonitor();
  virtual void Reset(){};
  virtual void Monitor(JadeDataFrameSP df);
 private:
  JadeOption m_opt;
  size_t m_ev_print;
  size_t m_ev_n;
};

using JadeMonitorSP = std::shared_ptr<JadeMonitor>;

#endif
