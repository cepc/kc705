#include "JadeOption.hh"

#include "json11.hpp"

JadeOption::JadeOption()
  :m_json_ptr(new json11::Json("")){
}

JadeOption::JadeOption(const std::string& str){
  size_t found = str.find_first_not_of(" \t\r\n");
  if(found != std::string::npos && str[found] !='{'){
    m_json_ptr = std::make_shared<json11::Json>(str);
  }
  else{
    std::string err_str;
    m_json_ptr = std::make_shared<json11::Json>(json11::Json::parse(str, err_str));
    if(!err_str.empty()){
      std::cerr<<"JadeOption: error<"<<err_str
	       <<">, unable to parse string: "<<str<<"\n";
      throw;
    }
  }
}

JadeOption::JadeOption(const char* ch){
  std::string str(ch);
  size_t found = str.find_first_not_of(" \t\r\n");
  if(found != std::string::npos && str[found] !='{'){
    m_json_ptr = std::make_shared<json11::Json>(str);
  }
  else{
    std::string err_str;
    m_json_ptr = std::make_shared<json11::Json>(json11::Json::parse(str, err_str));
    if(!err_str.empty()){
      std::cerr<<"JadeOption: error<"<<err_str
	       <<">, unable to parse string: "<<str<<"\n";
      throw;
    }
  }
}


JadeOption::JadeOption(const json11::Json& js)
  :m_json_ptr(new json11::Json(js)){
  
}

JadeOption::JadeOption(json11::Json&& js)
  :m_json_ptr(new json11::Json(std::move(js))){
  
}

JadeOption JadeOption::GetSubOption(const std::string& opt) const{
  return (*m_json_ptr)[opt];
}

std::map<std::string, JadeOption> JadeOption::GetSubMap() const{
  std::map<std::string, JadeOption> opt_map;
  for(auto& e: m_json_ptr->object_items()){
    opt_map[e.first] = JadeOption(e.second);
  }
  return opt_map;
}

std::string JadeOption::GetStringValue(const std::string& opt) const{
  if(opt.empty())
    return m_json_ptr->string_value();
  else{
    return (*m_json_ptr)[opt].string_value();
  }
}

int32_t JadeOption::GetIntValue(const std::string& opt) const{
  if(opt.empty())
    return m_json_ptr->int_value();
  else{
    return (*m_json_ptr)[opt].int_value();
  }
}


bool JadeOption::GetBoolValue(const std::string& opt) const{
  if(opt.empty())
    return m_json_ptr->bool_value();
  else{
    return (*m_json_ptr)[opt].bool_value();
  }
}

double JadeOption::GetFloatValue(const std::string& opt) const{
  if(opt.empty())
    return m_json_ptr->number_value();
  else{
    return (*m_json_ptr)[opt].number_value();
  }
}

std::string JadeOption::DumpString() const{
  return m_json_ptr->dump();
}
