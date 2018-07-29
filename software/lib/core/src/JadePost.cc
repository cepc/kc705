#include "JadePost.hh"
#include "JadeUtils.hh"

JadePost::~JadePost(){

}


JadeOption JadePost::Post(const std::string &url, const JadeOption &opt){
  try{
    std::size_t pos = url.find_first_of('/', 1);
    std::string url_folder = url.substr(0, pos);
    std::string url_remain = url.substr(pos);
    JadePostSP post = m_map_folder_post.at(url_folder).lock();
    if(post){
      return post->Post(url_remain, opt);
    }
    else{
      std::cout<<"JadePost: ERROR, resource is removed"<<std::endl;
      return "{\"status\":false}";
    }
  }
  catch (const std::out_of_range& oor) {
    std::cerr << "JadePost: ERROR, Out of Range:<"
              << oor.what() << "> url<"<<url<<">\n";
    return "{\"status\":false}";
  }
  std::cout<<"TODO, something"<<std::endl;
  return "{\"status\":false}";
}

void JadePost::AddSubPost(JadePostSP post){
  if(!post)
    return;
  std::string url_folder = "/"+JadeUtils::NameDemangle(typeid(*post).name());
  m_map_folder_post[url_folder]=post;
}

JadePostWP JadePost::s_sys_post;

void JadePost::SetSystemPost(JadePostSP post){
  s_sys_post = post;
}

JadePostSP JadePost::GetSystemPost(){
  return s_sys_post.lock();
}
