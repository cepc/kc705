#include "EudaqManager.hh"

JadeOption EudaqManager::Post(const std::string &url, const JadeOption &opt){    
  if(url == "/eudaq_init"){
    
    return "{\"status\":true}";
  }
  if(url == "/eudaq_config"){
      
    return "{\"status\":true}";
  }
  if(url == "/eudaq_start"){
    JadeManager::DeviceConnect();
    JadeManager::DeviceControl("START");
    JadeManager::StartDataTaking();
    return "{\"status\":true}";
  }
  if(url == "/eudaq_stop"){
    JadeManager::StopDataTaking();
    JadeManager::DeviceControl("STOP");
    JadeManager::DeviceDisconnect();  
    return "{\"status\":ture}";
  }
  if(url == "/eudaq_reset"){
    // JadeManager::reset();
    return "{\"status\":true}";
  }
  if(url == "/eudaq_terminate"){
    // JadeManager::reset();
    //TODO
    return "{\"status\":true}";
  }

  static const std::string url_base_class("/JadeManager/");
  if( ! url.compare(0, url_base_class.size(), url_base_class) ){
    return JadeManager::Post(url.substr(url_base_class.size()-1, opt));
  }
  return JadeManager::Post(url, opt));
}
