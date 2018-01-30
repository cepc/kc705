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

JadeRead::JadeRead(const std::string& path,
		   const std::string options)
  :m_dev_path(), m_options(options){
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

std::vector<JadeDataFrameUP>
JadeRead::Read(size_t nframe,
	       const std::chrono::milliseconds &timeout){
  size_t size_buf = FRAME_SIZE * nframe;
  m_buf.resize(size_buf);
  std::vector<JadeDataFrameUP> v_df;
  v_df.reserve(nframe);
  
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
    return v_df;

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

  size_t sub_beg = 0;
  while(sub_beg + FRAME_SIZE <  size_filled){
    v_df.emplace_back(new JadeDataFrame(m_buf.substr(sub_beg, FRAME_SIZE)));
    sub_beg += FRAME_SIZE;
  }
}
