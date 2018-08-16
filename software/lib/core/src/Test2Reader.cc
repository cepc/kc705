#include "JadeReader.hh"
#include "JadeUtils.hh"
#include "JadeFactory.hh"

#define FRAME_SIZE (4+96*(4+16*2+4)+4)

class Test2Reader: public JadeReader{
public:
  Test2Reader(const JadeOption &opt);
  ~Test2Reader() override {};
  JadeOption Post(const std::string &url, const JadeOption &opt) override;

  std::vector<JadeDataFrameSP> Read(size_t nframe,
                                    const std::chrono::milliseconds &timeout) override;
  JadeDataFrameSP Read(const std::chrono::milliseconds &timeout) override;
  void Open() override;
  void Close() override;
private:
  JadeOption m_opt;
  int m_fd;
  std::string m_buf;
  std::string m_path;
};

namespace{
  auto _test_index_ = JadeUtils::SetTypeIndex(std::type_index(typeid(Test2Reader)));
  auto _test_ = JadeFactory<JadeReader>::Register<Test2Reader, const JadeOption&>(typeid(Test2Reader));
}

Test2Reader::Test2Reader(const JadeOption& opt)
  :JadeReader(opt), m_opt(opt), m_fd(0){
}

void Test2Reader::Open(){
  if(m_fd){
    m_buf.clear();
#ifdef _WIN32
    _close(m_fd);
#else
    close(m_fd);
#endif
    m_fd = 0;
  }

  if(m_path.empty())
    m_path = m_opt.GetStringValue("PATH");
#ifdef _WIN32
  m_fd = _open(m_path.c_str(), _O_RDONLY | _O_BINARY);
#else
  m_fd = open(m_path.c_str(), O_RDONLY);
#endif
  if(m_fd <= 0){
    std::cerr<<"JadeRead: Failed to open devfile: "<<m_path<<"\n";
    m_fd = 0;
    throw;
  }
}

void Test2Reader::Close(){
  m_buf.clear();
  if(m_fd){
#ifdef _WIN32
    _close(m_fd);
#else
    close(m_fd);
#endif
    m_fd = 0;
  }
}


JadeDataFrameSP Test2Reader::Read(const std::chrono::milliseconds &timeout){
  std::cerr<<"Test2Reader: ERROR, the read function is not implimented \n";
  throw;
}

std::vector<JadeDataFrameSP>
Test2Reader::Read(size_t nframe,
	       const std::chrono::milliseconds &timeout){
  size_t size_buf = FRAME_SIZE * nframe;
  m_buf.resize(size_buf);
  std::vector<JadeDataFrameSP> v_df;
  v_df.reserve(nframe);

  size_t size_filled = 0;
  std::chrono::system_clock::time_point tp_timeout;
  bool can_time_out = false;
  uint32_t n = 0;
  uint32_t n_next = 4;
  while(size_filled < size_buf){
#ifdef _WIN32
    if(n+1 == n_next){
      std::cout<<n<<"  "<<size_filled<<"  "<<size_buf<<"  "<<(unsigned int)(size_buf-size_filled)<<std::endl;
    }
    int read_r = _read(m_fd, &m_buf[size_filled], (unsigned int)(size_buf-size_filled));
    n++;
    if(n == n_next){
      std::cout<<n<<std::endl;
      n_next = n_next*2;
    }
#else
    int read_r = read(m_fd, &m_buf[size_filled], size_buf-size_filled);
#endif
    if(read_r < 0){
      std::cerr<<"JadeRead: reading error\n";
      throw;
    }

    if(read_r == 0){
      std::cout<<"read_r==0"<<std::endl;
      if(!can_time_out){
	can_time_out = true;
	tp_timeout = std::chrono::system_clock::now() + timeout;
      }
      else{
	if(std::chrono::system_clock::now() > tp_timeout){
	  std::cerr<<"JadeRead: reading timeout\n";
	  //TODO: keep remain data, nothrow
	  throw;
	}
      }
      continue;
    }
    size_filled += read_r;
    can_time_out = false;
  }
  size_t sub_beg = 0;
  while(sub_beg + FRAME_SIZE <=  size_filled){
    v_df.emplace_back(new JadeDataFrame(m_buf.substr(sub_beg, FRAME_SIZE)));
    sub_beg += FRAME_SIZE;
  }  
  return v_df;
}


JadeOption Test2Reader::Post(const std::string &url, const JadeOption &opt){
  if(url == "/set_path"){
    m_path=opt.GetStringValue("PATH");
    return "{\"status\":true}";
  }
  
  static const std::string url_base_class("/JadeReader/");
  if( ! url.compare(0, url_base_class.size(), url_base_class) ){
    return JadeReader::Post(url.substr(url_base_class.size()-1), opt);
  }
  return JadePost::Post(url, opt);
}
