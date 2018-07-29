#ifndef JADE_JADEWRITE_HH
#define JADE_JADEWRITE_HH

#include "JadeSystem.hh"
#include "JadeFactory.hh"
#include "JadePost.hh"
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


class DLLEXPORT JadeWrite: public JadePost{
 public:
  JadeWrite(const JadeOption &opt);
  ~JadeWrite() override;
  static JadeWriteSP Make(const std::string& name, const JadeOption& opt);  
  JadeOption Post(const std::string &url, const JadeOption &opt) override;

  //open file for written 
  virtual void Open() {};
  //close file
  virtual void Close() {};
  //write data to the current openning file
  virtual void Write(JadeDataFrameSP df);
};



#endif
