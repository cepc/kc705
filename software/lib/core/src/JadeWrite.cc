#include "JadeWrite.hh"
#include <ctime>


JadeWrite::JadeWrite(const JadeOption &opt)
  :m_opt(opt), m_fd(0){
}

JadeWrite::~JadeWrite(){
  Reset();
}

void JadeWrite::Open(){
  std::string path = m_opt.GetStringValue("PATH");
  std::time_t time_now = std::time(nullptr);
  char time_buff[13];
  time_buff[12] = 0;
  std::strftime(time_buff, sizeof(time_buff),
		"%y%m%d%H%M%S", std::localtime(&time_now));
  std::string time_str(time_buff);
  std::string data_path = path+"_"+time_str +".df";
  m_fd = std::fopen(data_path.c_str(), "wb" );
  if(m_fd == NULL){
    std::cerr<<"JadeWrite: Failed to open/create file: "<<data_path<<"\n";
    throw;
  }
}

void JadeWrite::Reset(){
  Close();
}

void JadeWrite::Close(){
  if(m_fd){
    std::fclose(m_fd);
    m_fd = 0;
  }
}

void JadeWrite::Write(JadeDataFrameSP df){
  if(!df){
    std::cerr<<"JadeWrite: File is not opened/created before writing\n";
    throw;
  }
  std::string &rawstring = df->RawDataString();
  if(rawstring.size()){
    std::fwrite(&(rawstring.at(0)), 1, rawstring.size(), m_fd);
  }
}
