#ifndef JADE_JADEREGCTRL_HH
#define JADE_JADEREGCTRL_HH

#include "JadeSystem.hh"

#include <string>
#include <map>

class DLLEXPORT JadeRegCtrl{
 public:
  JadeRegCtrl(const std::string&,
	      const std::map<std::string, std::pair<uint16_t, uint8_t>>&);
  JadeRegCtrl(const std::string&);
  virtual ~JadeRegCtrl();
  void WriteByte(uint16_t addr, uint8_t val);
  uint8_t ReadByte(uint16_t addr);
  void SendCommand(const std::string &cmd);
  void SendCommand(const std::string &cmd, uint8_t val);
  uint8_t GetStatus(const std::string &cmd);
  bool WaitStatus(const std::string &cmd, std::chrono::milliseconds timeout);
  
 private:
  std::string m_dev_path;
  std::map<std::string, std::pair<uint16_t, uint8_t>> m_cmd_map;
};

using JadeRegCtrlSP = std::shared_ptr<JadeRegCtrl>;

#endif
