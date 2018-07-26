#include "JadeRegCtrl.hh"
#include "JadeUtils.hh"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

#include <thread>
#include <algorithm>

using _base_c_ = JadeRegCtrl;
using _index_c_ = JadeRegCtrl;

template class DLLEXPORT JadeFactory<_base_c_>;
template DLLEXPORT
std::unordered_map<std::type_index, typename JadeFactory<_base_c_>::UP (*)(const JadeOption&)>&
JadeFactory<_base_c_>::Instance<const JadeOption&>();

namespace{
  auto _loading_index_ = JadeUtils::SetTypeIndex(std::type_index(typeid(_index_c_)));
  auto _loading_ = JadeFactory<_base_c_>::Register<_base_c_, const JadeOption&>(typeid(_index_c_));
}


JadeRegCtrl::JadeRegCtrl(const JadeOption &opt)
  :m_opt(opt), m_fd(0), m_is_fd_read(false), m_is_fd_write(false){
}

JadeRegCtrl::~JadeRegCtrl(){
  Reset();
}

void JadeRegCtrl::Open(){
  std::string path = m_opt.GetStringValue("PATH");
  if(m_fd){
    close(m_fd);
    m_fd = 0;
    m_is_fd_read = false;
    m_is_fd_write = false;
  }
#ifdef _WIN32
  m_fd = _open(path.c_str(), _O_RDONLY | _O_BINARY);
#else
  m_fd = open(path.c_str(), O_RDONLY);
#endif
  if(m_fd < 0){
    std::cerr<<"JadeRegCtrl: Failed to open devfile by read mode: "<<path<<"\n";
    throw;
  }
  close(m_fd);
  m_fd = 0;
  m_is_fd_read = false;
  m_is_fd_write = false;
  
#ifdef _WIN32
  m_fd = _open(path.c_str(), _O_WRONLY | _O_BINARY);
#else
  m_fd = open(path.c_str(), O_WRONLY);
#endif
  if(m_fd < 0){
    std::cerr<<"JadeRegCtrl: Failed to open devfile by write mode: "<<path<<"\n";
    throw;
  }
  m_is_fd_read = false;
  m_is_fd_write = true;
}

void JadeRegCtrl::Close(){
  if(m_fd){
    close(m_fd);
    m_fd = 0;
    m_is_fd_read = false;
    m_is_fd_write = false;
  }
}

void JadeRegCtrl::Reset(){
  Close();
}

void JadeRegCtrl::WriteByte(uint16_t addr, uint8_t val){
  if(!m_fd || !m_is_fd_write){
    if(m_fd){
      close(m_fd);
      m_fd = 0;
      m_is_fd_read = false;
      m_is_fd_write = false;
    }
    std::string path = m_opt.GetStringValue("PATH");
#ifdef _WIN32
    m_fd = _open(path.c_str(), _O_WRONLY | _O_BINARY);
#else
    m_fd = open(path.c_str(), O_WRONLY);
#endif
    if(m_fd < 0){
      std::cerr<<"JadeRegCtrl: Failed to open devfile by write mode: "<<path<<"\n";
      throw;
    }
    m_is_fd_read = false;
    m_is_fd_write = true;
  }
  
#ifdef _WIN32
  auto lseek_r = _lseek(m_fd, addr, SEEK_SET);
#else 
  auto lseek_r = lseek(m_fd, addr, SEEK_SET);
#endif
  if(lseek_r < 0){
    std::cerr<<"JadeRegCtrl: Failed to seek devfile in write mode to the postion "<<addr<<"\n";
  }

#ifdef _WIN32
  auto write_r = _write(m_fd, &val, 1);
#else
  auto write_r = write(m_fd, &val, 1);
#endif
  if(write_r < 0) {
    std::cerr<<"JadeRegCtrl: Failed to write devfile\n";
    throw;
  }
}

uint8_t JadeRegCtrl::ReadByte(uint16_t addr){
  if(!m_fd || !m_is_fd_read){
    if(m_fd){
      close(m_fd);
      m_fd = 0;
      m_is_fd_read = false;
      m_is_fd_write = false;
    }
    std::string path = m_opt.GetStringValue("PATH");
#ifdef _WIN32
    m_fd = _open(path.c_str(), _O_RDONLY | _O_BINARY);
#else
    m_fd = open(path.c_str(), O_RDONLY);
#endif
    if(m_fd < 0){
      std::cerr<<"JadeRegCtrl: Failed to open devfile by read mode: "<<path<<"\n";
      throw;
    }
    m_is_fd_read = true;
    m_is_fd_write = false;
  }

#ifdef _WIN32
  auto lseek_r = _lseek(m_fd, addr, SEEK_SET);
#else 
  auto lseek_r = lseek(m_fd, addr, SEEK_SET);
#endif
  if(lseek_r < 0){
    std::cerr<<"JadeRegCtrl: Failed to seek devfile in read mode to the postion "<<addr<<"\n";
    throw;
  }
  
  uint8_t val;
#ifdef _WIN32
  auto read_r = _read(m_fd, &val, 1);
#else
  auto read_r = read(m_fd, &val, 1);
#endif
  if(read_r < 0) {
    std::cerr<<"JadeRegCtrl: Failed to read devfile\n";
    throw;
  }
  return val;
}

void JadeRegCtrl::SendCommand(const std::string &cmd){
  JadeOption cmd_opt =  m_opt.GetSubOption("command_list").GetSubOption(cmd);
  uint16_t addr = cmd_opt.GetIntValue("address");
  uint8_t default_val = cmd_opt.GetIntValue("default_val");
  WriteByte(addr, default_val);
}

void JadeRegCtrl::SendCommand(const std::string &cmd, uint8_t val){
  JadeOption cmd_opt =  m_opt.GetSubOption("command_list").GetSubOption(cmd);
  uint16_t addr = cmd_opt.GetIntValue("address");
  WriteByte(addr, val);
}

std::string JadeRegCtrl::GetStatus(const std::string &type){
  JadeOption type_opt = m_opt.GetSubOption("status_list").GetSubOption(type);
  JadeOption value_list_opt = type_opt.GetSubOption("value_list");
  std::map<std::string, JadeOption> opt_map = type_opt.GetSubMap();
  std::map<int32_t, std::string> status_map;
  for(auto& e: opt_map){
    status_map[e.second.GetIntValue()] = e.first;
  }
  uint16_t addr = type_opt.GetIntValue("address");
  uint8_t val = ReadByte(addr);
  std::string str_status = status_map[val];
  if(str_status.empty())
    str_status = "UNKNOWN_STATUS";
  return str_status;
}
