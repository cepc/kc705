#ifndef JADE_JADEWRITE_HH
#define JADE_JADEWRITE_HH

#include "JadeSystem.hh"
#include "JadeDataFrame.hh"
#include "JadeOption.hh"

#include <string>
#include <chrono>
#include <mutex>
#include <queue>

#include <sys/types.h>
#include <sys/stat.h>


class DLLEXPORT JadeWrite{
 public:
  JadeWrite(const JadeOption &opt);
  virtual ~JadeWrite();
  virtual void Open(){};
  virtual void Reset(){};
  void Write(JadeDataFrameSP df);
 private:
  FILE* m_fd;
  JadeOption m_opt;
};

using JadeWriteSP = std::shared_ptr<JadeWrite>;

#endif
