#ifndef JADE_JADEWRITE_HH
#define JADE_JADEWRITE_HH

#include "JadeSystem.hh"
#include "JadeFactory.hh"

#include "JadeDataFrame.hh"
#include "JadeOption.hh"

#include <string>
#include <chrono>
#include <mutex>
#include <queue>

#include <sys/types.h>
#include <sys/stat.h>

class JadeWrite;
using JadeWriteSP = JadeFactory<JadeWrite>::SP;
using JadeWriteUP = JadeFactory<JadeWrite>::UP;


#ifndef JADE_DLL_EXPORT
extern template class DLLEXPORT JadeFactory<JadeWrite>;
extern template DLLEXPORT
std::unordered_map<std::type_index, typename JadeFactory<JadeWrite>::UP (*)(const JadeOption&)>&
JadeFactory<JadeWrite>::Instance<const JadeOption&>();
#endif


class DLLEXPORT JadeWrite{
 public:
  JadeWrite(const JadeOption &opt);
  virtual ~JadeWrite();
  virtual void Open();
  virtual void Close();
  virtual void Reset();
  void Write(JadeDataFrameSP df);
  
  static JadeWriteSP Make(const std::string& type, const JadeOption& opt);

 private:
  FILE* m_fd;
  JadeOption m_opt;
  bool m_disable_file_write;
};





#endif
