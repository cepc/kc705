#ifndef JADE_UTILS_HH_
#define JADE_UTILS_HH_

#include <typeindex>
#include <unordered_map>
#include <map>
#include <string>
#include <chrono>

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
};

#endif
