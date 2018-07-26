#ifndef JADE_JADEREGCTRL_HH
#define JADE_JADEREGCTRL_HH

#include "JadeSystem.hh"
#include "JadeFactory.hh"
#include "JadeOption.hh"

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

class DLLEXPORT JadeRegCtrl{
 public:
  JadeRegCtrl(const JadeOption &opt);
  virtual ~JadeRegCtrl();
  virtual void Open();
  virtual void Close();
  virtual void Reset();
  virtual void WriteByte(uint16_t addr, uint8_t val);
  virtual uint8_t ReadByte(uint16_t addr);
  void SendCommand(const std::string &cmd);
  void SendCommand(const std::string &cmd, uint8_t val);
  std::string GetStatus(const std::string &cmd);
 private:
  JadeOption m_opt;
  int m_fd;
  bool m_is_fd_read;
  bool m_is_fd_write;
};

#endif 
