#ifndef _JADE_JADEUTILS_HH_
#define _JADE_JADEUTILS_HH_

#include <typeindex>
#include <unordered_map>
#include <map>
#include <string>
#include <chrono>
#include <memory>
#include <iostream>
#include <string>
#include <cstdio>

class JadeUtils{
public:
  static std::type_index GetTypeIndex(const std::string& name);
  static bool SetTypeIndex(const std::type_index& index);
  static bool SetTypeIndex(const std::string& name, const std::type_index& index);
  static std::string NameDemangle( const std::string&  man);
  static std::unordered_map<std::string, std::type_index>& TypeIndexMap();
  static void PrintTypeIndexMap();
  static std::string LoadFileToString(const std::string &path);
  static std::string GetNowStr();
  static std::string GetBinaryPath();
  static bool LoadBinary(const std::string& file);

  template<typename ... Args>
  static std::string FormatString( const std::string& format, Args ... args ){
    std::size_t size = snprintf( nullptr, 0, format.c_str(), args ... ) + 1;
    std::unique_ptr<char[]> buf( new char[ size ] ); 
    std::snprintf( buf.get(), size, format.c_str(), args ... );
    return std::string( buf.get(), buf.get() + size - 1 );
  }
  
};

#endif
