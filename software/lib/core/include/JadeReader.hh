#ifndef _JADE_JADEREAD_HH_
#define _JADE_JADEREAD_HH_

#include "JadeSystem.hh"
#include "JadeFactory.hh"
#include "JadeOption.hh"
#include "JadePost.hh"
#include "JadeUtils.hh"

#include "JadeDataFrame.hh"

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#include <string>
#include <chrono>
#include <mutex>
#include <queue>

class JadeReader;
using JadeReaderSP = JadeFactory<JadeReader>::SP;
using JadeReaderUP = JadeFactory<JadeReader>::UP;

#ifndef JADE_DLL_EXPORT
extern template class DLLEXPORT JadeFactory<JadeReader>;
extern template DLLEXPORT
std::unordered_map<std::type_index, typename JadeFactory<JadeReader>::UP (*)(const JadeOption&)>&
JadeFactory<JadeReader>::Instance<const JadeOption&>();
#endif

class DLLEXPORT JadeReader: public JadePost{
 public:
  JadeReader(const JadeOption &opt);
  ~JadeReader() override;
  static JadeReaderSP Make(const std::string&name, const JadeOption &opt);
  JadeOption Post(const std::string &url, const JadeOption &opt) override;

  //open data device for read
  virtual void Open(){};
  //close data device
  virtual void Close(){};
  //read maximum 'nframe' of data with 'timeout'
  virtual std::vector<JadeDataFrameSP> Read(size_t nframe, const std::chrono::milliseconds &timeout);
  //read a data frame with 'timeout'
  virtual JadeDataFrameSP Read(const std::chrono::milliseconds &timeout);
};


#endif
