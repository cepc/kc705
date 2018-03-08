#include "JadeWrite.hh"

JadeWrite::JadeWrite(const JadeOption &opt)
  :m_opt(opt){
  std::string path = opt.GetStringValue("PATH");
  m_fd = std::fopen(path.c_str(), "wb" );
  if(m_fd == NULL){
    std::cerr<<"JadeWrite: Failed to open devfile: "<<path<<"\n";
    throw;
  }
}

JadeWrite::~JadeWrite(){
  if(m_fd)
    std::fclose(m_fd);
}

void JadeWrite::Write(JadeDataFrameSP df){
  if(!df)
    throw;
  std::string &rawstring = df->RawDataString();
  if(rawstring.size()){
    std::fwrite(&(rawstring.at(0)), 1, rawstring.size(), m_fd);
  }
}
