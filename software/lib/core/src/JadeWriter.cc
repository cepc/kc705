#include "JadeWriter.hh"
#include <ctime>

#include "JadeUtils.hh"

using _base_c_ = JadeWriter;
using _index_c_ = JadeWriter;

template class DLLEXPORT JadeFactory<_base_c_>;
template DLLEXPORT
std::unordered_map<std::type_index, typename JadeFactory<_base_c_>::UP (*)(const JadeOption&)>&
JadeFactory<_base_c_>::Instance<const JadeOption&>();

namespace{
  auto _loading_index_ = JadeUtils::SetTypeIndex(std::type_index(typeid(_index_c_)));
  auto _loading_ = JadeFactory<_base_c_>::Register<_index_c_, const JadeOption&>(typeid(_index_c_));
}

JadeWriter::JadeWriter(const JadeOption &opt){
}

JadeWriter::~JadeWriter(){
}

JadeWriterSP JadeWriter::Make(const std::string& name, const JadeOption& opt){  
  try{
    std::type_index index = JadeUtils::GetTypeIndex(name);
    JadeWriterSP wrt =  JadeFactory<JadeWriter>::MakeUnique<const JadeOption&>(index, opt);
    return wrt;
  }
  catch(...){
    std::cout<<"TODO"<<std::endl;
    return nullptr;
  }
}

void JadeWriter::Write(JadeDataFrameSP df){
}

JadeOption JadeWriter::Post(const std::string &url, const JadeOption &opt){
  return JadePost::Post(url, opt);
}

//++++++++++++++++++++++++++++++++++
//TestWriter.hh
class TestWriter: public JadeWriter{
 public:
  TestWriter(const JadeOption &opt);
  ~TestWriter() override {}; 
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
//TestWriter.cc
namespace{
  auto _test_index_ = JadeUtils::SetTypeIndex(std::type_index(typeid(TestWriter)));
  auto _test_ = JadeFactory<JadeWriter>::Register<TestWriter, const JadeOption&>(typeid(TestWriter));
}

TestWriter::TestWriter(const JadeOption &opt)
  :m_opt(opt), m_fd(0), m_disable_file_write(false), JadeWriter(opt){
  m_disable_file_write = m_opt.GetBoolValue("DISABLE_FILE_WRITE");
  m_path = m_opt.GetStringValue("PATH"); 
}

void TestWriter::Open(){
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

void TestWriter::Close(){
  if(m_disable_file_write)
    return;
  if(m_fd){
    std::fclose(m_fd);
    m_fd = 0;
  }
}

void TestWriter::Write(JadeDataFrameSP df){
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

JadeOption TestWriter::Post(const std::string &url, const JadeOption &opt){  
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

  static const std::string url_base_class("/JadeWriter/");
  if( ! url.compare(0, url_base_class.size(), url_base_class) ){
    return JadeWriter::Post(url.substr(url_base_class.size()-1), opt);
  }
  return JadePost::Post(url, opt);
}
