#ifndef JADE_JADEMANAGER_HH
#define JADE_JADEMANAGER_HH

#include "JadeSystem.hh"
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

class DLLEXPORT JadeManager{
 public:
  JadeManager();
  virtual ~JadeManager();

  void SetRegCtrl(JadeRegCtrlSP ctrl);
  void SetReader(JadeReadSP rd);
  void SetWriter(JadeWriteSP wrt);
  void SetFilter(JadeFilterSP flt);
  void SetMonitor(JadeMonitorSP mnt);

  void SetRegCtrl(const std::string &arg){};
  void SetReader(const std::string &arg){};
  void SetWriter(const std::string &arg){};
  void SetFilter(const std::string &arg){};
  void SetMonitor(const std::string &arg){};
  
  void Start();
  void Stop();
  void Control(const std::string &arg);
 private:
  uint64_t AsyncReading();
  uint64_t AsyncDecoding();
  uint64_t AsyncFiltering();
  uint64_t AsyncWriting();
  uint64_t AsyncMonitoring();
  
 private:
  JadeRegCtrlSP m_ctrl;
  JadeReadSP m_rd;
  JadeFilterSP m_flt;
  JadeWriteSP m_wrt;
  JadeMonitorSP m_mnt;
  
  bool m_is_running;
  std::future<uint64_t> m_fut_async_rd;
  std::future<uint64_t> m_fut_async_dcd;
  std::future<uint64_t> m_fut_async_flt;
  std::future<uint64_t> m_fut_async_wrt;
  std::future<uint64_t> m_fut_async_mnt;
  std::mutex m_mx_ev_to_dcd;
  std::mutex m_mx_ev_to_flt;
  std::mutex m_mx_ev_to_wrt;
  std::mutex m_mx_ev_to_mnt;
  std::queue<JadeDataFrameSP> m_qu_ev_to_dcd;
  std::queue<JadeDataFrameSP> m_qu_ev_to_flt;
  std::queue<JadeDataFrameSP> m_qu_ev_to_wrt;
  std::queue<JadeDataFrameSP> m_qu_ev_to_mnt;
  std::condition_variable m_cv_valid_ev_to_dcd;
  std::condition_variable m_cv_valid_ev_to_flt;
  std::condition_variable m_cv_valid_ev_to_wrt;
  std::condition_variable m_cv_valid_ev_to_mnt;
};

#endif
