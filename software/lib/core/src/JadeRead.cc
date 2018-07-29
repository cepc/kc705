#include "JadeRead.hh"
#include "JadeUtils.hh"

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <thread>

using _base_c_ = JadeRead;
using _index_c_ = JadeRead;

template class DLLEXPORT JadeFactory<_base_c_>;
template DLLEXPORT
std::unordered_map<std::type_index, typename JadeFactory<_base_c_>::UP (*)(const JadeOption&)>&
JadeFactory<_base_c_>::Instance<const JadeOption&>();

namespace{
  auto _loading_index_ = JadeUtils::SetTypeIndex(std::type_index(typeid(_index_c_)));
  auto _loading_ = JadeFactory<_base_c_>::Register<_index_c_, const JadeOption&>(typeid(_index_c_));
}


JadeRead::JadeRead(const JadeOption& opt){
}

JadeRead::~JadeRead(){
}

JadeReadSP JadeRead::Make(const std::string& name, const JadeOption& opt){  
  try{
    std::type_index index = JadeUtils::GetTypeIndex(name);
    JadeReadSP wrt =  JadeFactory<JadeRead>::MakeUnique<const JadeOption&>(index, opt);
    return wrt;
  }
  catch(...){
    std::cout<<"TODO"<<std::endl;
    return nullptr;
  }
}

JadeOption JadeRead::Post(const std::string &url, const JadeOption &opt){
  return JadePost::Post(url, opt);
}

std::vector<JadeDataFrameSP> JadeRead::Read(size_t nframe,
                                            const std::chrono::milliseconds &timeout){
  std::this_thread::sleep_for(timeout);
  return {};
}

JadeDataFrameSP JadeRead::Read(const std::chrono::milliseconds &timeout){
  std::this_thread::sleep_for(timeout);
  return nullptr;
}

//+++++++++++++++++++++++++++++++++++++++++
//TestRead.hh

class TestRead: public JadeRead{
public:
  TestRead(const JadeOption &opt);
  ~TestRead() override {};
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
};

//+++++++++++++++++++++++++++++++++++++++++
//TestRead.cc
namespace{
  auto _test_index_ = JadeUtils::SetTypeIndex(std::type_index(typeid(TestRead)));
  auto _test_ = JadeFactory<JadeRead>::Register<TestRead, const JadeOption&>(typeid(TestRead));
}


TestRead::TestRead(const JadeOption& opt)
  : m_opt(opt), m_fd(0), JadeRead(opt){
}


void TestRead::Open(){
  if (m_fd) {
    m_buf.clear();
#ifdef _WIN32
    _close(m_fd);
#else
    close(m_fd);
#endif
    m_fd = 0;
  }

  std::string path = m_opt.GetStringValue("PATH");
#ifdef _WIN32
  m_fd = _open(path.c_str(), _O_RDONLY | _O_BINARY);
#else
  m_fd = open(path.c_str(), O_RDONLY);
#endif
  if (m_fd <= 0) {
    std::cerr << "JadeRead: Failed to open devfile: " << path << "\n";
    m_fd = 0;
    throw;
  }
}

void TestRead::Close(){
  m_buf.clear();
  if (m_fd) {
#ifdef _WIN32
    _close(m_fd);
#else
    close(m_fd);
#endif
    m_fd = 0;
  }
}

std::vector<JadeDataFrameSP>
TestRead::Read(size_t nframe,
    const std::chrono::milliseconds& timeout)
{
  std::vector<JadeDataFrameSP> v_df;
  for (size_t i = 0; i < nframe; i++) {
    v_df.push_back(Read(timeout));
  }
  return v_df;
}


JadeDataFrameSP TestRead::Read(const std::chrono::milliseconds& timeout)
{
  uint32_t size_pack;
  size_pack = sizeof(size_pack);
  size_t size_buf = size_pack;
  if (m_buf.size() < size_buf) {
    m_buf.resize(size_buf);
  }
  size_t size_filled = 0;
  std::chrono::system_clock::time_point tp_timeout;
  bool can_time_out = false;
  uint32_t n = 0;
  uint32_t n_next = 4;

  while (size_filled < size_buf) {
#ifdef _WIN32
    if (n + 1 == n_next) {
      std::cout << n << "  " << size_filled << "  " << size_buf << "  " << (unsigned int)(size_buf - size_filled) << std::endl;
    }
    int read_r = _read(m_fd, &m_buf[size_filled], (unsigned int)(size_buf - size_filled));
    n++;
    if (n == n_next) {
      std::cout << n << std::endl;
      n_next = n_next * 2;
    }
#else
    int read_r = read(m_fd, &m_buf[size_filled], size_buf - size_filled);
#endif
    if (read_r < 0) {
      std::cerr << "JadeRead: reading error\n";
      throw;
    }
    if (read_r == 0) {
      std::cout << "read_r==0" << std::endl;
      if (!can_time_out) {
        can_time_out = true;
        tp_timeout = std::chrono::system_clock::now() + timeout;
      }
      else {
        if (std::chrono::system_clock::now() > tp_timeout) {
          std::cerr << "JadeRead: reading timeout\n";
          //TODO: keep remain data, nothrow
          throw;
        }
      }
      continue;
    }
    size_filled += read_r;
    if (size_filled == sizeof(size_pack)) {
      size_pack = LE32TOH(*(reinterpret_cast<uint32_t*>(&m_buf[0])));
      size_buf = size_pack;
      m_buf.resize(size_buf);
    }
    can_time_out = false;
  }
  return JadeDataFrameSP(new JadeDataFrame(m_buf.substr(0, size_pack)));
}

JadeOption TestRead::Post(const std::string &url, const JadeOption &opt){    
  if(url == "/open"){
    Open();
    return "{\"status\":ture}";
  }
  
  if(url == "/close"){
    Close();
    return "{\"status\":true}";
  }

  static const std::string url_base_class("/JadeRead/");
  if( ! url.compare(0, url_base_class.size(), url_base_class) ){
    return JadeRead::Post(url.substr(url_base_class.size()-1), opt);
  }
  return JadePost::Post(url, opt);
}
