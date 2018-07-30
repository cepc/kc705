#include "JadeWrite.hh"
#include <ctime>

#include "JadeUtils.hh"

using _base_c_ = JadeWrite;
using _index_c_ = JadeWrite;

template class DLLEXPORT JadeFactory<_base_c_>;
template DLLEXPORT
std::unordered_map<std::type_index, typename JadeFactory<_base_c_>::UP (*)(const JadeOption&)>&
JadeFactory<_base_c_>::Instance<const JadeOption&>();

namespace{
  auto _loading_index_ = JadeUtils::SetTypeIndex(std::type_index(typeid(_index_c_)));
  auto _loading_ = JadeFactory<_base_c_>::Register<_index_c_, const JadeOption&>(typeid(_index_c_));
}

JadeWrite::JadeWrite(const JadeOption &opt){
}

JadeWrite::~JadeWrite(){
}

JadeWriteSP JadeWrite::Make(const std::string& name, const JadeOption& opt){  
  try{
    std::type_index index = JadeUtils::GetTypeIndex(name);
    JadeWriteSP wrt =  JadeFactory<JadeWrite>::MakeUnique<const JadeOption&>(index, opt);
    return wrt;
  }
  catch(...){
    std::cout<<"TODO"<<std::endl;
    return nullptr;
  }
}

void JadeWrite::Write(JadeDataFrameSP df){
}

JadeOption JadeWrite::Post(const std::string &url, const JadeOption &opt){
  return JadePost::Post(url, opt);
}

//++++++++++++++++++++++++++++++++++
//TestWrite.hh
class TestWrite: public JadeWrite{
 public:
  TestWrite(const JadeOption &opt);
  ~TestWrite() override {}; 
  JadeOption Post(const std::string &url, const JadeOption &opt) override;

  void Open() override;
  void Close() override;
  void Write(JadeDataFrameSP df) override;
 private:
  FILE* m_fd;
  std::string m_path;
  JadeOption m_opt;
  bool m_disable_file_write;
};

//+++++++++++++++++++++++++++++++++++++++++
//TestWrite.cc
namespace{
  auto _test_index_ = JadeUtils::SetTypeIndex(std::type_index(typeid(TestWrite)));
  auto _test_ = JadeFactory<JadeWrite>::Register<TestWrite, const JadeOption&>(typeid(TestWrite));
}

TestWrite::TestWrite(const JadeOption &opt)
  :m_opt(opt), m_fd(0), m_disable_file_write(false), JadeWrite(opt){
  m_disable_file_write = m_opt.GetBoolValue("DISABLE_FILE_WRITE");
  m_path = m_opt.GetStringValue("PATH"); 
}

void TestWrite::Open(){
  if(m_disable_file_write)
    return;
  std::time_t time_now = std::time(nullptr);
  char time_buff[13];
  time_buff[12] = 0;
  std::strftime(time_buff, sizeof(time_buff),
		"%y%m%d%H%M%S", std::localtime(&time_now));
  std::string time_str(time_buff);
  std::string data_path = m_path+"_"+time_str +".df";
  m_fd = std::fopen(data_path.c_str(), "wb" );
  if(m_fd == NULL){
    std::cerr<<"JadeWrite: Failed to open/create file: "<<data_path<<"\n";
    throw;
  }
}

void TestWrite::Close(){
  if(m_disable_file_write)
    return;
  if(m_fd){
    std::fclose(m_fd);
    m_fd = 0;
  }
}

void TestWrite::Write(JadeDataFrameSP df){
  if(m_disable_file_write)
    return;
  if(!df){
    std::cerr<<"JadeWrite: File is not opened/created before writing\n";
    throw;
  }
  std::string &rawstring = df->RawDataString();
  if(rawstring.size()){
    std::fwrite(&(rawstring.at(0)), 1, rawstring.size(), m_fd);
  }
}

JadeOption TestWrite::Post(const std::string &url, const JadeOption &opt){  
  if(url == "/close"){
    Close();
    return "{\"status\":true}";
  }

  if(url == "/open"){
    Open();
    return "{\"status\":true}";
  }
  
  if(url == "/reload_opt"){
    Close();
    m_opt = opt;
    m_disable_file_write = m_opt.GetBoolValue("DISABLE_FILE_WRITE");
    m_path = m_opt.GetStringValue("PATH");
    return "{\"status\":true}";
  }

  static const std::string url_base_class("/JadeWrite/");
  if( ! url.compare(0, url_base_class.size(), url_base_class) ){
    return JadeWrite::Post(url.substr(url_base_class.size()-1), opt);
  }
  return JadePost::Post(url, opt);
}
