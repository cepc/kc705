#include "JadeUtils.hh"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#include <intrin.h>
#pragma intrinsic(_ReturnAddress)
#else
#include <dlfcn.h>
#endif

std::unordered_map<std::string, std::type_index>& JadeUtils::TypeIndexMap(){
  static std::unordered_map<std::string, std::type_index> s_um_name_typeindex;
  return s_um_name_typeindex;
}

std::type_index JadeUtils::GetTypeIndex(const std::string& name){
  return TypeIndexMap().at(name);
}

bool JadeUtils::SetTypeIndex(const std::string& name, const std::type_index& index){
  if(TypeIndexMap().count(name)) {
    std::cerr<<"JadeUtil:: ERROR, name "
             << name
             << " have been registered by "
             << TypeIndexMap().at(name).name()<<"\n";
    return false;
  }
  TypeIndexMap().count(name);
  TypeIndexMap().insert({name, index});
  return true;  
}

bool JadeUtils::SetTypeIndex(const std::type_index& index){
  std::string demangled_name = NameDemangle(index.name());
  return SetTypeIndex(demangled_name, index);
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

void JadeUtils::PrintTypeIndexMap(){
  auto& um_name_typeindex = TypeIndexMap();
  std::cout<<"\n\n===list of type index===\n";
  for(auto &item: um_name_typeindex){
    std::cout<<item.first<<" = "<<item.second.name()<<"\n";
  }
  std::cout<<"\n\n"<<std::endl;
}


std::string JadeUtils::LoadFileToString(const std::string& path){
  std::ifstream ifs(path);
  if(!ifs.good()){
    std::cerr<<"JadeUtils: ERROR, unable to load file<"<<path<<">\n";
    throw;
  }
  
  std::string str;
  str.assign((std::istreambuf_iterator<char>(ifs) ),
             (std::istreambuf_iterator<char>()));
  return str;
}

std::string JadeUtils::GetNowStr(){
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss<<std::put_time(std::localtime(&now_c), "%c");
    return ss.str();
}

std::string JadeUtils::GetBinaryPath(){
#ifdef _WIN32
  void* address_return = _ReturnAddress();
  HMODULE handle = NULL;
  ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS
                      |GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                      static_cast<LPCSTR>(address_return), &handle);
  char modpath[MAX_PATH] = {'\0'};
  ::GetModuleFileNameA(handle, modpath, MAX_PATH);
  return modpath;
#else
  void* address_return = (void*)(__builtin_return_address(0));
  Dl_info dli;
  dli.dli_fname = 0;
  dladdr(address_return, &dli);
  return dli.dli_fname;
#endif
}


bool JadeUtils::LoadBinary(const std::string& file){
  void *handle;
#ifdef _WIN32
  handle = (void *)LoadLibrary(file.c_str());
#else
  handle = dlopen(file.c_str(), RTLD_NOW);
#endif
  if(handle){
    // m_modules[file]=handle;
    return true;
  }
  else{
    std::cerr<<"JadeUtils: Fail to load module binary<"<<file<<">\n";
    char *errstr;
#ifdef _WIN32
    //copied from https://mail.python.org/pipermail//pypy-commit/2012-October/066804.html
    static char buf[32];
    DWORD dw = GetLastError();
    if (dw == 0)
      return NULL;
    sprintf(buf, "error 0x%x", (unsigned int)dw);
    //TODO get human readable https://msdn.microsoft.com/en-us/library/windows/desktop/ms679351(v=vs.85).aspx
    errstr = buf;
#else
    errstr = dlerror();
#endif	    
    printf ("A dynamic linking error occurred: (%s)\n", errstr); //Give the poor user some way to know what went wrong...
    return false;
  }
}

int base64_atob( const std::string &in_str, std::string &out_str );
int base64_btoa(const std::string &in_str, std::string &out_str, int linesize);

std::string JadeUtils::Base64_btoa(const std::string &bin){
  std::string base64;
  base64_btoa(bin, base64, 0);
  return base64;
}

std::string JadeUtils::Base64_atob(const std::string &base64){
  std::string bin;
  base64_atob(base64, bin);
  return bin;
}

