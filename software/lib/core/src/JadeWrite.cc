#include "JadeWrite.hh"

JadeWrite::JadeWrite(const JadeOption &opt)
  :m_opt(opt), m_fd(0){
}

JadeWrite::~JadeWrite(){
  Reset();
}

void JadeWrite::Open(){
  std::string path = m_opt.GetStringValue("PATH");
  m_fd = std::fopen(path.c_str(), "wb" );
  if(m_fd == NULL){
    std::cerr<<"JadeWrite: Failed to open/create file: "<<path<<"\n";
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
