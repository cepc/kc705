#ifndef JADE_JADEREGCTRL_HH
#define JADE_JADEREGCTRL_HH

#include "JadeSystem.hh"
#include "JadeOption.hh"

#include <string>
#include <map>

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
  const std::string& GetStatus(const std::string &cmd);
 private:
  JadeOption m_opt;
  int m_fd;
  bool m_is_fd_read;
  bool m_is_fd_write;
  std::map<std::string, std::pair<uint16_t, uint8_t>> m_cmd_map; //TODO: keep it as json
  std::map<std::string, std::pair<uint16_t, std::map<uint8_t, std::string>>> m_status_map;
};

using JadeRegCtrlSP = std::shared_ptr<JadeRegCtrl>;

#endif 
