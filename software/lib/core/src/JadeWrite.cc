#include "JadeWrite.hh"

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

JadeWrite::JadeWrite(const std::string& path,
		    const std::string options)
  :m_path(), m_options(options){
#ifdef _WIN32
  m_fd = _open(m_path.c_str(), O_WRONLY | _O_BINARY);
#else
  m_fd = open(m_path.c_str(), O_WRONLY);
#endif
  if(m_fd < 0){
    std::cerr<<"Failed to open devfile";
  }
}

JadeWrite::~JadeWrite(){
#ifdef _WIN32
  _close(m_fd);
#else
  close(m_fd);
#endif
}

void JadeWrite::Write(JadeDataFrameUP &&df){
  std::string &rawstring = df->RawDataString();
  if(rawstring.size()){
#ifdef _WIN32
    _write(m_fd, &(rawstring.at(0)), rawstring.size());
#else
    write(m_fd, &(rawstring.at(0)), rawstring.size());
#endif
  }   
}
