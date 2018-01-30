#include "JadeDump.hh"


#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

JadeDump::JadeDump(const std::string& dev_path)
  :m_dev_path(dev_path){
  m_dummy_buf.resize(4096);
}

JadeDump::~JadeDump(){

}

size_t JadeDump::Dump(const std::chrono::milliseconds &timeout){
  std::chrono::system_clock::time_point tp_timeout;
#ifdef _WIN32
  int fd = _open(m_dev_path.c_str(), _O_RDONLY | _O_BINARY);
#else
  int fd = open(m_dev_path.c_str(), O_RDONLY);
#endif
  if(fd < 0){
    std::cerr<<"JadeDump: Failed to open devfile";
  }

  bool is_timer_started = false;
  while(1){
#ifdef _WIN32
    int read_r = _read(fd, &m_dummy_buf[0], m_dummy_buf.size());
#else
    int read_r = read(fd, &m_dummy_buf[0], m_dummy_buf.size());
#endif
    if(read_r <0){
      std::cerr<<"JadeDump: reading error";
      break;
    }
    else if(read_r > 0){
      is_timer_started = false;
      continue;
    }
    else{
      if(!is_timer_started){
	tp_timeout = std::chrono::system_clock::now() + timeout;
	is_timer_started = true;
	continue;
      }
      else{
	if(tp_timeout < std::chrono::system_clock::now())
	  break;
      }
    }
  }
  return 0;
}
