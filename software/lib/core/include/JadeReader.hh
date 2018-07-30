#ifndef JADE_JADEREAD_HH
#define JADE_JADEREAD_HH

#include "JadeSystem.hh"
#include "JadeFactory.hh"
#include "JadeOption.hh"
#include "JadePost.hh"

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

class DLLEXPORT JadeRead: public JadePost{
 public:
  JadeRead(const JadeOption &opt);
  ~JadeRead() override;
  static JadeReadSP Make(const std::string&name, const JadeOption &opt);
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
