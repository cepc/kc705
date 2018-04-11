#ifndef JADE_JADEREAD_HH
#define JADE_JADEREAD_HH

#include "JadeSystem.hh"
#include "JadeOption.hh"
#include "JadeDataFrame.hh"

#include <string>
#include <chrono>
#include <mutex>
#include <queue>

class DLLEXPORT JadeRead{
 public:
  JadeRead(const JadeOption &opt);
  virtual ~JadeRead();
  virtual void Open();
  virtual void Close();
  virtual void Reset();
  virtual std::vector<JadeDataFrameSP> Read(size_t nframe,
					    const std::chrono::milliseconds &timeout);
  virtual JadeDataFrameSP Read(const std::chrono::milliseconds &timeout);

 private:
  JadeOption m_opt;
  int m_fd;
  std::string m_buf;
};

using JadeReadSP = std::shared_ptr<JadeRead>;

#endif
