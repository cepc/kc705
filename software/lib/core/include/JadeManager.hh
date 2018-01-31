#ifndef JADE_JADEMANAGER_HH
#define JADE_JADEMANAGER_HH

#include "JadeSystem.hh"
#include "JadeDataFrame.hh"
#include "JadeRead.hh"
#include "JadeFilter.hh"
#include "JadeWrite.hh"

#include <string>
#include <queue>
#include <mutex>
#include <future>
#include <condition_variable>

class DLLEXPORT JadeManager{
 public:
  JadeManager();
  ~JadeManager();

  void Start(const std::string &file_in,
	     const std::string &file_out);
 private:
  uint64_t AsyncReading();
  uint64_t AsyncDecoding();
  uint64_t AsyncFiltering();
  uint64_t AsyncWriting();
  
 private:
  std::unique_ptr<JadeRead> m_rd;
  std::unique_ptr<JadeFilter> m_flt;
  std::unique_ptr<JadeWrite> m_wrt;
  
  bool m_is_running;
  std::future<uint64_t> m_fut_async_rd;
  std::future<uint64_t> m_fut_async_dcd;
  std::future<uint64_t> m_fut_async_flt;
  std::future<uint64_t> m_fut_async_wrt;
  std::mutex m_mx_ev_to_dcd;
  std::mutex m_mx_ev_to_flt;
  std::mutex m_mx_ev_to_wrt;
  std::queue<JadeDataFrameUP> m_qu_ev_to_dcd;
  std::queue<JadeDataFrameUP> m_qu_ev_to_flt;
  std::queue<JadeDataFrameUP> m_qu_ev_to_wrt;
  std::condition_variable m_cv_valid_ev_to_dcd;
  std::condition_variable m_cv_valid_ev_to_flt;
  std::condition_variable m_cv_valid_ev_to_wrt;
};

#endif
