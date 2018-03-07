#ifndef JADE_JADEOPTION_HH
#define JADE_JADEOPTION_HH

#include "JadeSystem.hh"

namespace json11{ class Json;}
class JadeOption;
using JadeOptionSP = std::shared_ptr<JadeOption>;

class DLLEXPORT JadeOption{
 public:
  JadeOption(const std::string& str);
  JadeOption(const json11::Json& js);
  std::string DumpString();
  JadeOptionSP GetSubOption(const std::string& key);
  std::string GetStringValue(const std::string& key = "");
  bool GetBoolValue(const std::string& key = "");
  int32_t GetIntValue(const std::string& key = "" );
  double GetFloatValue(const std::string& key = "");
 private:
  std::shared_ptr<json11::Json> m_json_ptr;
};


#endif
