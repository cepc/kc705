#include "JadePost.hh"

JadePost::~JadePost(){

}


JadeOption JadePost::Post(const std::string &url, const JadeOption &opt){
  if(url == "/dump_opt"){
    std::cout<< opt.DumpString()<<std::endl;
    return "{\"status\":true}";
  }
  else{
    return "{\"status\":false}";
  }
}
