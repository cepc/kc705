#ifndef JADE_JADEOPTION_HH
#define JADE_JADEOPTION_HH

#include "JadeSystem.hh"

namespace json11{ class Json;}
class JadeOption;

class DLLEXPORT JadeOption{
 public:
  JadeOption();
  JadeOption(const std::string& str);
  JadeOption(const json11::Json& js);
  std::string DumpString() const;
  JadeOption GetSubOption(const std::string& key) const;
  std::string GetStringValue(const std::string& key = "") const;
  bool GetBoolValue(const std::string& key = "") const;
  int32_t GetIntValue(const std::string& key = "" ) const;
  double GetFloatValue(const std::string& key = "") const;
 private:
  std::shared_ptr<json11::Json> m_json_ptr;
};


#endif
