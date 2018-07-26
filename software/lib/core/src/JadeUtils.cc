#include "JadeUtils.hh"

#include <iostream>

std::unordered_map<std::string, std::type_index> JadeUtils::s_um_name_typeindex;

const std::type_index JadeUtils::GetTypeIndex(const std::string& name){
  return s_um_name_typeindex.at(name);
}

bool JadeUtils::SetTypeIndex(const std::string& name, const std::type_index& index){
  if(s_um_name_typeindex.count(name)) {
    std::cerr<<"JadeUtil:: ERROR, name "
             << name
             << " have been registered by "
             << s_um_name_typeindex.at(name).name()<<"\n";
    return false;
  }
  s_um_name_typeindex.insert({name, index});
  return true;
  
}

extern "C" {
char* __cxa_demangle(const char* mangled_name,
                     char* buf,
                     size_t* n,
                     int* status);

typedef void* (*malloc_func_t)(size_t);
typedef void (*free_func_t)(void*);
char* __unDName(char* buffer,
                const char* mangled,
                int buflen,
                malloc_func_t memget,
                free_func_t memfree,
                unsigned short int flags);

}

std::string JadeUtils::NameDemangle(const std::string& mang) {
  const static std::string zprefix_s("__Z");
  const static std::string zprefix_l("____Z");

  const char* s = mang.c_str();
  const char* cxa_in = s;
  
  if (mang.compare(0, zprefix_s.size() , zprefix_s) == 0 ||
      mang.compare(0, zprefix_l.size() , zprefix_l) == 0 ){
    cxa_in += 1;
  }
  
  if (char* lx = __cxa_demangle(cxa_in, NULL, NULL, NULL)) {
    std::string demang(lx);    
    free(lx);
    return demang;
  } else if (char* ms = __unDName(NULL, s, 0, &malloc, &free, 0)) {
    std::string demang(ms);
    free(ms);
    return demang;
  } else {
    return mang;
  }  
}
