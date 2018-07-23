#ifndef JADE_JADEMONITOR_HH
#define JADE_JADEMONITOR_HH

#include "JadeDataFrame.hh"
#include "JadeOption.hh"
#include "JadeSystem.hh"

class DLLEXPORT JadeMonitor {
  public:
  JadeMonitor(const JadeOption& opt);
  virtual ~JadeMonitor();
  virtual void Reset();
  virtual void Monitor(JadeDataFrameSP df);

  private:
  JadeOption m_opt;
  size_t m_ev_print;
  size_t m_ev_n;
  bool m_enable_print_discon;
  size_t m_last_df_n;
  JadeDataFrameSP m_last_df;
  bool m_enable_cds;
};

using JadeMonitorSP = std::shared_ptr<JadeMonitor>;

#endif
