#include "JadeRead.hh"

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FRAME_SIZE 1024

JadeRead::JadeRead(const std::string& dev_path,
		   const std::string options){
  
#ifdef _WIN32
  m_fd = _open(m_dev_path.c_str(), O_RDONLY | _O_BINARY);
#else
  m_fd = open(m_dev_path.c_str(), O_RDONLY);
#endif
  if(m_fd < 0){
    perror("Failed to open devfile");
  }
}

JadeRead::~JadeRead(){
#ifdef _WIN32
  _close(m_fd);
#else
  close(m_fd);
#endif
}

void JadeRead::Read(std::queue<JadeDataFrameUP> &qu, std::mutex &mx,
		    size_t nframe, std::chrono::milliseconds timeout){
  size_t size_buf = FRAME_SIZE * nframe;
  m_buf.resize(size_buf);
  size_t size_filled = 0;
  std::chrono::system_clock::time_point tp_first_pause;
  std::chrono::system_clock::time_point tp_timeout;
  while(size_filled < size_buf){
#ifdef _WIN32
    int read_r = _read(m_fd, &m_buf[size_filled], size_buf-size_filled);
#else
    int read_r = read(m_fd, &m_buf[size_filled], size_buf-size_filled);
#endif
    if(read_r <= 0)
      std::cerr<<"JadeRead: error";
    return;

    if(read_r < size_buf-size_filled){
      if(size_filled == 0){
	tp_first_pause = std::chrono::system_clock::now();
	tp_timeout = tp_first_pause + timeout;
      }
      size_filled += read_r;
      auto tp_now = std::chrono::system_clock::now();
      if(tp_now > tp_timeout){
	std::cerr<<"JadeRead: timeout";
	break;
      }
    }
    else{
      size_filled += read_r;
      break;
    }
  }

  size_t substr_beg = 0;
  while(substr_beg + FRAME_SIZE <  size_filled){
    auto df = JadeDataFrameUP(new JadeDataFrame(m_buf.substr(0, FRAME_SIZE)));
    substr_beg += FRAME_SIZE;
    std::lock_guard<std::mutex> lk(mx);
      qu.push(std::move(df));
  }
}
