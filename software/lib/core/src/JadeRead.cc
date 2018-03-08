#include "JadeRead.hh"

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FRAME_SIZE (4+48*(4+16*2+4)+4)

JadeRead::JadeRead(const JadeOption &opt)
  :m_opt(opt){
}

JadeRead::~JadeRead(){
  Reset();
}

void JadeRead::Open(){
  std::string path = m_opt.GetStringValue("PATH");
#ifdef _WIN32
  m_fd = _open(path.c_str(), _O_RDONLY | _O_BINARY);
#else
  m_fd = open(path.c_str(), O_RDONLY);
#endif
  if(m_fd < 0){
    std::cerr<<"JadeRead: Failed to open devfile: "<<path<<"\n";
    throw;
  }
}

void JadeRead::Close(){
  m_buf.clear();
  if(m_fd>0){
#ifdef _WIN32
    _close(m_fd);
#else
    close(m_fd);
#endif
    m_fd = 0;
  }
}

void JadeRead::Reset(){
  Close();
}

std::vector<JadeDataFrameSP>
JadeRead::Read(size_t nframe,
	       const std::chrono::milliseconds &timeout){
  size_t size_buf = FRAME_SIZE * nframe;
  m_buf.resize(size_buf);
  std::vector<JadeDataFrameSP> v_df;
  v_df.reserve(nframe);
  
  size_t size_filled = 0;
  std::chrono::system_clock::time_point tp_timeout;
  while(size_filled < size_buf){
#ifdef _WIN32
    int read_r = _read(m_fd, &m_buf[size_filled], (unsigned int)(size_buf-size_filled));
#else
    int read_r = read(m_fd, &m_buf[size_filled], size_buf-size_filled);
#endif
    if(read_r < 0){
      std::cerr<<"JadeRead: reading error\n";
      throw;
    }

    if(read_r < size_buf-size_filled){
      if(size_filled == 0){
	tp_timeout = std::chrono::system_clock::now() + timeout;
      }
      size_filled += read_r;
      if(std::chrono::system_clock::now() > tp_timeout){
	std::cerr<<"JadeRead: reading timeout\n";
	//TODO: keep remain data, nothrow
	throw;
      }
    }
    else{
      size_filled += read_r;
      break;
    }
  }

  size_t sub_beg = 0;
  while(sub_beg + FRAME_SIZE <=  size_filled){
    v_df.emplace_back(new JadeDataFrame(m_buf.substr(sub_beg, FRAME_SIZE)));
    sub_beg += FRAME_SIZE;
  }  
  return v_df;
}
