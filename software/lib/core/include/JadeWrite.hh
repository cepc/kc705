#ifndef JADE_JADEWRITE_HH
#define JADE_JADEWRITE_HH

#include "JadeSystem.hh"
#include "JadeDataFrame.hh"
#include <string>
#include <chrono>
#include <mutex>
#include <queue>

class DLLEXPORT JadeWrite{
 public:
  JadeWrite(const std::string& path, const std::string options);
  ~JadeWrite();
  void Write(JadeDataFrameUP &&df);
 private:
  int m_fd;
  std::string m_path;
  std::string m_options;
};

#endif
