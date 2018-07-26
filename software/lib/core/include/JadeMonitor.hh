#ifndef JADE_JADEMONITOR_HH
#define JADE_JADEMONITOR_HH

#include "JadeSystem.hh"
#include "JadeFactory.hh"
#include "JadeDataFrame.hh"
#include "JadeOption.hh"


class JadeMonitor;
using JadeMonitorSP = JadeFactory<JadeMonitor>::SP;
using JadeMonitorUP = JadeFactory<JadeMonitor>::UP;

#ifndef JADE_DLL_EXPORT
extern template class DLLEXPORT JadeFactory<JadeMonitor>;
extern template DLLEXPORT
std::unordered_map<std::type_index, typename JadeFactory<JadeMonitor>::UP (*)(const JadeOption&)>&
JadeFactory<JadeMonitor>::Instance<const JadeOption&>();
#endif

class DLLEXPORT JadeMonitor{
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
};


#endif
