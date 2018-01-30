#ifndef JADE_JADEDUMP_HH
#define JADE_JADEDUMP_HH

#include "JadeSystem.hh"
#include <string>

class DLLEXPORT JadeDump{
 public:
  JadeDump(const std::string& dev_path);
  ~JadeDump();
  uint64_t Dump(uint32_t time_ms);
 private:
  std::string m_dev_path;
};

#endif
