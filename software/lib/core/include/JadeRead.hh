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
  virtual ~JadeRead();
  virtual std::vector<JadeDataFrameSP> Read(size_t nframe,
					    const std::chrono::milliseconds &timeout);
 private:
  int m_fd;
  std::string m_dev_path;
  std::string m_options;
  std::string m_buf;
};

using JadeReadSP = std::shared_ptr<JadeRead>;

#endif
