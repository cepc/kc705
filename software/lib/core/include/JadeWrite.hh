#ifndef JADE_JADEWRITE_HH
#define JADE_JADEWRITE_HH

#include "JadeSystem.hh"
#include "JadeDataFrame.hh"
#include <string>
#include <chrono>
#include <mutex>
#include <queue>

#include <sys/types.h>
#include <sys/stat.h>


class DLLEXPORT JadeWrite{
 public:
  JadeWrite(const std::string& path, const std::string options);
  ~JadeWrite();
  void Write(JadeDataFrameUP &&df);
 private:
  FILE* m_fd;
  std::string m_path;
  std::string m_options;
};

#endif
