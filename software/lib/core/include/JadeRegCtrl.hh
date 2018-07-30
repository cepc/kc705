#ifndef JADE_JADEREGCTRL_HH
#define JADE_JADEREGCTRL_HH

#include "JadeSystem.hh"
#include "JadeFactory.hh"
#include "JadeOption.hh"
#include "JadePost.hh"
#include "JadeUtils.hh"


#include <string>
#include <map>

class JadeRegCtrl;
using JadeRegCtrlSP = JadeFactory<JadeRegCtrl>::SP;
using JadeRegCtrlUP = JadeFactory<JadeRegCtrl>::UP;

#ifndef JADE_DLL_EXPORT
extern template class DLLEXPORT JadeFactory<JadeRegCtrl>;
extern template DLLEXPORT
std::unordered_map<std::type_index, typename JadeFactory<JadeRegCtrl>::UP (*)(const JadeOption&)>&
JadeFactory<JadeRegCtrl>::Instance<const JadeOption&>();
#endif


class DLLEXPORT JadeRegCtrl: public JadePost{
 public:
  JadeRegCtrl(const JadeOption &opt);
  ~JadeRegCtrl() override;
  static JadeRegCtrlSP Make(const std::string& name, const JadeOption &opt);
  JadeOption Post(const std::string &url, const JadeOption &opt) override;

  //open controller device;
  virtual void Open() {};
  //close device;
  virtual void Close() {};
  //send command with parameter to current openning controller device  
  virtual std::string SendCommand(const std::string &cmd, const std::string &para) {return "";};

  virtual std::string SendCommand(const std::string &cmd) final{
    return SendCommand(cmd, "");
  }
};

#endif 
