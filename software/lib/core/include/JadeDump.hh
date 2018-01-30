#ifndef JADE_JADEDUMP_HH
#define JADE_JADEDUMP_HH

#include "JadeSystem.hh"
#include <string>
#include <chrono>

class DLLEXPORT JadeDump{
 public:
  JadeDump(const std::string& dev_path);
  ~JadeDump();
  size_t Dump(const std::chrono::milliseconds &timeout);
 private:
  std::string m_dev_path;
  std::string m_dummy_buf;
};

#endif
