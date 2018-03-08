#include "JadeOption.hh"

#include "json11.hpp"

JadeOption::JadeOption(const std::string& str){
  std::string err_str;
  m_json_ptr = std::make_shared<json11::Json>(json11::Json::parse(str, err_str));
  if(!err_str.empty()){
    std::cerr<<"JadeOption: error<"<<err_str<<">, unable to parse string: "
	     <<str<<"\n";
    throw;
  }
}

JadeOption::JadeOption(const json11::Json& js){
  m_json_ptr.reset(new json11::Json(js));
}

JadeOptionSP JadeOption::GetSubOption(const std::string& opt){
  return std::make_shared<JadeOption>((*m_json_ptr)[opt]);  
}

std::string JadeOption::GetStringValue(const std::string& opt){
  if(opt.empty())
    return m_json_ptr->string_value();
  else{
    return (*m_json_ptr)[opt].string_value();
  }
}


int32_t JadeOption::GetIntValue(const std::string& opt){
  if(opt.empty())
    return m_json_ptr->int_value();
  else{
    return (*m_json_ptr)[opt].int_value();
  }
}


bool JadeOption::GetBoolValue(const std::string& opt){
  if(opt.empty())
    return m_json_ptr->bool_value();
  else{
    return (*m_json_ptr)[opt].bool_value();
  }
}

double JadeOption::GetFloatValue(const std::string& opt){
  if(opt.empty())
    return m_json_ptr->number_value();
  else{
    return (*m_json_ptr)[opt].number_value();
  }
}

std::string JadeOption::DumpString(){
  return m_json_ptr->dump();
}
