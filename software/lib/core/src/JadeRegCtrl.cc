#include "JadeRegCtrl.hh"

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


JadeRegCtrl::JadeRegCtrl(const JadeOption &opt)
  :m_opt(opt), m_fd(0), m_is_fd_read(false), m_is_fd_write(false){
  m_cmd_map={{"START",{3, 15}}, {"STOP",{4, 15}}, {"SET",{2, 10}}, {"CHIPA1",{8, 0}}, {"CHIPA2",{8, 1}},{"CHIPA3",{8, 2}}, {"CHIPA4",{8, 3}}, {"CHIPA5",{8, 4}}, {"CHIPA6",{8, 5}}, {"CHIPA7",{8, 6}}, {"CHIPA8",{8, 7}}, {"CHIPA9",{8, 8}}, {"CHIPA10",{8, 9}}};
}

JadeRegCtrl::~JadeRegCtrl(){
  Reset();
}

void JadeRegCtrl::Open(){
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
  std::string str = cmd;
  std::transform(str.begin(), str.end(), str.begin(), ::toupper);
  auto addr_val = m_cmd_map.at(str);
  WriteByte(addr_val.first, addr_val.second);
}

void JadeRegCtrl::SendCommand(const std::string &cmd, uint8_t val){
  std::string str = cmd;
  std::transform(str.begin(), str.end(), str.begin(), ::toupper);
  auto addr_val = m_cmd_map.at(str);
  WriteByte(addr_val.first, val);
}

const std::string& JadeRegCtrl::GetStatus(const std::string &type){
  std::string str = type;
  std::transform(str.begin(), str.end(), str.begin(), ::toupper);
  auto& addr_status_pair = m_status_map.at(str);
  uint8_t val = ReadByte(addr_status_pair.first);
  return addr_status_pair.second.at(val); //TODO: if unknown
}
