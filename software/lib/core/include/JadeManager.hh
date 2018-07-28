#ifndef JADE_JADEMANAGER_HH
#define JADE_JADEMANAGER_HH

#include "JadeSystem.hh"
#include "JadeFactory.hh"
#include "JadeUtils.hh"
#include "JadeOption.hh"
#include "JadeDataFrame.hh"
#include "JadeRead.hh"
#include "JadeFilter.hh"
#include "JadeWrite.hh"
#include "JadeMonitor.hh"
#include "JadeRegCtrl.hh"

#include <string>
#include <queue>
#include <mutex>
#include <future>
#include <condition_variable>

class JadeManager;
using JadeManagerSP = JadeFactory<JadeManager>::SP;
using JadeManagerUP = JadeFactory<JadeManager>::UP;

#ifndef JADE_DLL_EXPORT
extern template class DLLEXPORT JadeFactory<JadeManager>;
extern template DLLEXPORT
std::unordered_map<std::type_index, typename JadeFactory<JadeManager>::UP (*)(const JadeOption&)>&
JadeFactory<JadeManager>::Instance<const JadeOption&>();
#endif

class DLLEXPORT JadeManager: public JadePost{
 public:
  JadeManager(const JadeOption &opt);
  virtual ~JadeManager();
  static JadeManagerSP Make(const std::string& name, const JadeOption& opt);
  void Reset();

  JadeOption Post(const std::string &url, const JadeOption &opt) override;


  
  //TODO: weak_ptr
  JadeRegCtrlSP GetRegCtrl() { return m_ctrl; };
  JadeReadSP GetReader() { return m_rd; };
  JadeWriteSP GetWriter() { return m_wrt; };
  JadeFilterSP GetFilter() { return m_flt; };
  JadeMonitorSP GetMonitor() { return m_mnt; };
  
  void StartDataTaking();
  void StopDataTaking();
  void DeviceConnect();
  void DeviceDisconnect();
  
  void DeviceControl(const std::string &cmd);
  std::string DeviceStatus(const std::string &type);
  
 private:
  uint64_t AsyncReading();
  uint64_t AsyncFiltering();
  uint64_t AsyncWriting();
  uint64_t AsyncMonitoring();
  
 private:
  JadeOption m_opt;
  
  JadeRegCtrlSP m_ctrl;
  JadeReadSP m_rd;
  JadeFilterSP m_flt;
  JadeWriteSP m_wrt;
  JadeMonitorSP m_mnt;
  
  bool m_is_running;
  std::future<uint64_t> m_fut_async_rd;
  std::future<uint64_t> m_fut_async_flt;
  std::future<uint64_t> m_fut_async_wrt;
  std::future<uint64_t> m_fut_async_mnt;
  std::mutex m_mx_ev_to_flt;
  std::mutex m_mx_ev_to_wrt;
  std::mutex m_mx_ev_to_mnt;
  std::queue<JadeDataFrameSP> m_qu_ev_to_flt;
  std::queue<JadeDataFrameSP> m_qu_ev_to_wrt;
  std::queue<JadeDataFrameSP> m_qu_ev_to_mnt;
  std::condition_variable m_cv_valid_ev_to_flt;
  std::condition_variable m_cv_valid_ev_to_wrt;
  std::condition_variable m_cv_valid_ev_to_mnt;    
};

#endif
