#ifndef JADE_JADEMONITOR_HH
#define JADE_JADEMONITOR_HH

#include "JadeSystem.hh"
#include "JadeFactory.hh"
#include "JadePost.hh"
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

class DLLEXPORT JadeMonitor: public JadePost{
 public:
  JadeMonitor(const JadeOption& opt);
  ~JadeMonitor() override;
  static JadeMonitorSP Make(const std::string& name, const JadeOption& opt);
  JadeOption Post(const std::string &url, const JadeOption &opt) override;

  //reset inner state, do it before start a new datataking.
  virtual void Reset(){};
  //put data into monitor
  virtual void Monitor(JadeDataFrameSP df);
  //the control panel of monitor, return information by string
  virtual std::string SendCommand(const std::string &cmd, const std::string &para){return "";};

  //the control panel of monitor, return information by string, no parameter version
  virtual std::string SendCommand(const std::string &cmd) final{
    return SendCommand(cmd, "");
  }
};

#endif
