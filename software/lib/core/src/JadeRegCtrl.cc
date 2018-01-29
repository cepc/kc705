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

JadeRegCtrl::JadeRegCtrl(const std::string& dev_path,
			 std::map<std::string, std::pair\
			 <uint16_t, uint8_t>> cmd_map)
  :m_dev_path(dev_path), m_cmd_map(cmd_map){
  
}

JadeRegCtrl::~JadeRegCtrl(){

}

void JadeRegCtrl::WriteByte(uint16_t addr, uint8_t val){
#ifdef _WIN32
  int fd = _open(m_dev_path.c_str(), O_WRONLY | _O_BINARY);
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
  int fd = _open(m_dev_path.c_str(), O_RDONLY | _O_BINARY);
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

void JadeRegCtrl::Command(const std::string &cmd){
  auto addr = m_cmd_map.at(cmd).first;
  auto val = m_cmd_map.at(cmd).second;
  WriteByte(addr, val);
}

void JadeRegCtrl::Command(const std::string &cmd, uint8_t val){
  auto addr = m_cmd_map.at(cmd).first;
  WriteByte(addr, val);
}
