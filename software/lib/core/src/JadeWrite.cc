#include "JadeWrite.hh"

JadeWrite::JadeWrite(const std::string& path,
		    const std::string options)
  :m_path(path), m_options(options){
  m_fd = std::fopen(m_path.c_str(), "wb" );
  if(m_fd == NULL){
    std::cerr<<"JadeWrite: Failed to open devfile: "<<m_path<<"\n";
    // throw;
  }
}

JadeWrite::~JadeWrite(){
  if(m_fd)
    std::fclose(m_fd);
}

void JadeWrite::Write(JadeDataFrameUP &&df){
  if(!df)
    throw;
  std::string &rawstring = df->RawDataString();
  if(rawstring.size()){
    std::fwrite(&(rawstring.at(0)), 1, rawstring.size(), m_fd);
    //std::cout<< "writing:"<< df->RawDataString().size()<<std::endl;
  }
}
