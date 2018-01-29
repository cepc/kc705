#ifndef JADE_JADEREAD_HH
#define JADE_JADEREAD_HH

#include "JadeSystem.hh"
#include "JadeDataFrame.hh"
#include <string>
#include <chrono>
#include <mutex>
#include <queue>

class DLLEXPORT JadeRead{
 public:
  JadeRead(const std::string& dev_path, const std::string options);
  ~JadeRead();
  
  void Read(std::queue<JadeDataFrameUP> &qu, std::mutex &mx,
	    size_t nframe, std::chrono::milliseconds timeout);
  
 private:
  int m_fd;
  std::string m_dev_path;
  std::string m_options;
  std::string m_buf;
};

#endif
