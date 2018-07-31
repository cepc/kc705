#ifndef JADE_JADEWRITE_HH
#define JADE_JADEWRITE_HH

#include "JadeSystem.hh"
#include "JadeFactory.hh"
#include "JadePost.hh"
#include "JadeDataFrame.hh"
#include "JadeOption.hh"
#include "JadeUtils.hh"

#include <string>
#include <chrono>
#include <mutex>
#include <queue>

#include <sys/types.h>
#include <sys/stat.h>

class JadeWriter;
using JadeWriterSP = JadeFactory<JadeWriter>::SP;
using JadeWriterUP = JadeFactory<JadeWriter>::UP;

#ifndef JADE_DLL_EXPORT
extern template class DLLEXPORT JadeFactory<JadeWriter>;
extern template DLLEXPORT
std::unordered_map<std::type_index, typename JadeFactory<JadeWriter>::UP (*)(const JadeOption&)>&
JadeFactory<JadeWriter>::Instance<const JadeOption&>();
#endif

class DLLEXPORT JadeWriter: public JadePost{
 public:
  JadeWriter(const JadeOption &opt);
  ~JadeWriter() override;
  static JadeWriterSP Make(const std::string& name, const JadeOption& opt);  
  JadeOption Post(const std::string &url, const JadeOption &opt) override;

  //open file for written 
  virtual void Open() {};
  //close file
  virtual void Close() {};
  //write data to the current openning file
  virtual void Write(JadeDataFrameSP df);
};



#endif
