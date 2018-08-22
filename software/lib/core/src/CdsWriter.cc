#include "JadeWriter.hh"

//++++++++++++++++++++++++++++++++++
//CdsWriter.hh
class CdsWriter: public JadeWriter{
  public:
    CdsWriter(const JadeOption &opt);
    ~CdsWriter() override {};
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
//CdsWriter.cc
namespace{
  auto _test_index_ = JadeUtils::SetTypeIndex(std::type_index(typeid(CdsWriter)));
  auto _test_ = JadeFactory<JadeWriter>::Register<CdsWriter, const JadeOption&>(typeid(CdsWriter));
}

CdsWriter::CdsWriter(const JadeOption &opt)
  :m_opt(opt), m_fd(0), m_disable_file_write(false), JadeWriter(opt){
    m_disable_file_write = m_opt.GetBoolValue("DISABLE_FILE_WRITE");
  }

void CdsWriter::Open(){
  if(m_path.empty())
    m_path = m_opt.GetStringValue("PATH");

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

void CdsWriter::Close(){
  if(m_disable_file_write)
    return;
  if(m_fd){
    std::fclose(m_fd);
    m_fd = 0;
  }
}

void CdsWriter::Write(JadeDataFrameSP df){
  if(m_disable_file_write)
    return;
  if(!df){
    std::cerr<<"JadeWrite: File is not opened/created before writing\n";
    throw;
  }
  auto& data = df->Data();
  if(data.size()){
    std::fwrite(reinterpret_cast<char*>(&(data[0])), 2, data.size(), m_fd);
  }
}

JadeOption CdsWriter::Post(const std::string &url, const JadeOption &opt){
  if(url == "/set_path"){
    m_path=opt.GetStringValue("PATH");
    return "{\"status\":true}";
  }

  static const std::string url_base_class("/JadeWriter/");
  if( ! url.compare(0, url_base_class.size(), url_base_class) ){
    return JadeWriter::Post(url.substr(url_base_class.size()-1), opt);
  }
  return JadePost::Post(url, opt);
}
