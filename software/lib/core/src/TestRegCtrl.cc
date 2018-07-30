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
#include <sstream>

//+++++++++++++++++++++++++++++++++++++++++
//TestRegCtrl.hh

class TestRegCtrl: public JadeRegCtrl{
public:
  TestRegCtrl(const JadeOption &opt);
  ~TestRegCtrl() override {};
  JadeOption Post(const std::string &url, const JadeOption &opt) override;

  void Open() override;
  void Close() override;
  std::string SendCommand(const std::string &cmd, const std::string &para) override;
private:
  void WriteByte(uint16_t addr, uint8_t val);
  uint8_t ReadByte(uint16_t addr);
  std::string GetStatus(const std::string &cmd);

private:
  JadeOption m_opt;
  int m_fd;
  bool m_is_fd_read;
  bool m_is_fd_write;
};

//+++++++++++++++++++++++++++++++++++++++++
//TestRegCtrl.cc
namespace{
  auto _test_index_ = JadeUtils::SetTypeIndex(std::type_index(typeid(TestRegCtrl)));
  auto _test_ = JadeFactory<JadeRegCtrl>::Register<TestRegCtrl, const JadeOption&>(typeid(TestRegCtrl));
}

TestRegCtrl::TestRegCtrl(const JadeOption &opt)
  :m_opt(opt), m_fd(0), m_is_fd_read(false), m_is_fd_write(false), JadeRegCtrl(opt){
}

void TestRegCtrl::Open(){
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

void TestRegCtrl::Close(){
  if(m_fd){
    close(m_fd);
    m_fd = 0;
    m_is_fd_read = false;
    m_is_fd_write = false;
  }
}

void TestRegCtrl::WriteByte(uint16_t addr, uint8_t val){
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

uint8_t TestRegCtrl::ReadByte(uint16_t addr){
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

std::string TestRegCtrl::SendCommand(const std::string &cmd, const std::string &para){
  if(cmd == "status"){
    return GetStatus(para);
  }
  if(para.empty()){
    JadeOption cmd_opt =  m_opt.GetSubOption("command_list").GetSubOption(cmd);
    uint16_t addr = cmd_opt.GetIntValue("address");
    uint8_t default_val = cmd_opt.GetIntValue("default_val");
    WriteByte(addr, default_val);
    return "";
  }
  else{
    std::stringstream ss;
    uint16_t val;
    ss<<para;
    ss>>val;
    JadeOption cmd_opt =  m_opt.GetSubOption("command_list").GetSubOption(cmd);
    uint16_t addr = cmd_opt.GetIntValue("address");
    WriteByte(addr, val);
    return "";
  }
}

std::string TestRegCtrl::GetStatus(const std::string &type){
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

JadeOption TestRegCtrl::Post(const std::string &url, const JadeOption &opt){
  if(url == "/get_status"){
    std::string st = GetStatus(opt.GetStringValue("type"));
    return JadeUtils::FormatString("{\"status\":ture, \"get_status\": %s}", st );
  }

  if(url == "/open"){
    Open();
    m_opt = opt;
    return "{\"status\":ture}";
  }
  
  if(url == "/close"){
    Close();
    m_opt = opt;
    return "{\"status\":ture}";
  }
    
  static const std::string url_base_class("/JadeRegCtrl/");
  if( ! url.compare(0, url_base_class.size(), url_base_class) ){
    return JadeRegCtrl::Post(url.substr(url_base_class.size()-1), opt);
  }
  return JadePost::Post(url, opt);
}
