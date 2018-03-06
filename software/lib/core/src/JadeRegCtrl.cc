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

JadeRegCtrl::JadeRegCtrl(const std::string& dev_path,
			 const std::map<std::string, std::pair\
			 <uint16_t, uint8_t>> &cmd_map)
  :m_dev_path(dev_path), m_cmd_map(cmd_map){
}

JadeRegCtrl::JadeRegCtrl(const std::string& dev_path)
  :m_dev_path(dev_path){
  m_cmd_map={{"START",{3, 15}}, {"STOP",{4, 15}}};  
}

JadeRegCtrl::~JadeRegCtrl(){

}

void JadeRegCtrl::WriteByte(uint16_t addr, uint8_t val){
#ifdef _WIN32
  int fd = _open(m_dev_path.c_str(), _O_WRONLY | _O_BINARY);
#else
  int fd = open(m_dev_path.c_str(), O_WRONLY);
#endif
  
  if (fd < 0) {
    perror("Failed to open devfile");
  }

#ifdef _WIN32
  auto lseek_r = _lseek(fd, addr, SEEK_SET);
#else 
  auto lseek_r = lseek(fd, addr, SEEK_SET);
#endif
  if(lseek_r < 0)
    perror("Failed to seek");

#ifdef _WIN32
  auto write_r = _write(fd, &val, 1);
#else
  auto write_r = write(fd, &val, 1);
#endif
  if(write_r < 0) {
    perror("allwrite() failed to write");
  }
  close(fd);
}

uint8_t JadeRegCtrl::ReadByte(uint16_t addr){
  uint8_t val;

#ifdef _WIN32
  int fd = _open(m_dev_path.c_str(), _O_RDONLY | _O_BINARY);
#else
  int fd = open(m_dev_path.c_str(), O_RDONLY);
#endif

  if (fd < 0) {
    perror("Failed to open devfile");
  }
#ifdef _WIN32
  auto lseek_r = _lseek(fd, addr, SEEK_SET);
#else 
  auto lseek_r = lseek(fd, addr, SEEK_SET);
#endif
  if(lseek_r < 0){
    perror("Failed to seek");
  }

#ifdef _WIN32
  auto read_r = _read(fd, &val, 1);
#else
  auto read_r = read(fd, &val, 1);
#endif
  if(read_r < 0) {
    perror("allread() failed to read");
  }
  close(fd);
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

uint8_t JadeRegCtrl::GetStatus(const std::string &cmd){
  std::string str = cmd;
  std::transform(str.begin(), str.end(), str.begin(), ::toupper);
  auto addr_val = m_cmd_map.at(str);
  return ReadByte(addr_val.first);
}

bool JadeRegCtrl::WaitStatus(const std::string &cmd, std::chrono::milliseconds timeout){
  std::string str = cmd;
  std::transform(str.begin(), str.end(), str.begin(), ::toupper);
  auto addr_val = m_cmd_map.at(str);
  uint16_t addr = addr_val.first;
  uint8_t val_expected = addr_val.first;
  auto tp_timeout =  std::chrono::system_clock::now() + timeout;
  while(true){
    uint8_t val_r = ReadByte(addr_val.first);
    if(val_r == val_expected){
      return true;
    }
    if(std::chrono::system_clock::now() > tp_timeout){
      return false;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
  }
  
}
