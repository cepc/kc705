#ifndef JADE_JADEREGCTRL_HH
#define JADE_JADEREGCTRL_HH

#include "JadeSystem.hh"

#include <string>
#include <map>

class DLLEXPORT JadeRegCtrl{
 public:
  JadeRegCtrl(const std::string&,
	      std::map<std::string, std::pair<uint16_t, uint8_t>>);
  virtual ~JadeRegCtrl();
  
  void WriteByte(uint16_t addr, uint8_t val);
  uint8_t ReadByte(uint16_t addr);
  void Command(const std::string &cmd);
  void Command(const std::string &cmd, uint8_t val);
  
 private:
  std::string m_dev_path;
  std::map<std::string, std::pair<uint16_t, uint8_t>> m_cmd_map;
};

#endif
