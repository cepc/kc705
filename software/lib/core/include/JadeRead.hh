#ifndef JADE_JADEREAD_HH
#define JADE_JADEREAD_HH

#include "JadeSystem.hh"
#include "JadeFactory.hh"
#include "JadeOption.hh"
#include "JadeDataFrame.hh"

#include <string>
#include <chrono>
#include <mutex>
#include <queue>

class JadeRead;
using JadeReadSP = JadeFactory<JadeRead>::SP;
using JadeReadUP = JadeFactory<JadeRead>::UP;

#ifndef JADE_DLL_EXPORT
extern template class DLLEXPORT JadeFactory<JadeRead>;
extern template DLLEXPORT
std::unordered_map<std::type_index, typename JadeFactory<JadeRead>::UP (*)(const JadeOption&)>&
JadeFactory<JadeRead>::Instance<const JadeOption&>();
#endif

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


#endif
