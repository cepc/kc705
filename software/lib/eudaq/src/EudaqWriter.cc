#include "EudaqWriter.hh"


EudaqWriter::EudaqWriter(const JadeOption& opt)
  :JadeWrite(opt){
  

}

~EudaqWriter::EudaqWriter(){


}



void SetJadeManagerCallback(JadeManagerSP man){
  
}

void EudaqWriter::Write(JadeDataFrameSP df){
  //ev = df;
  //SendEvent(std::move(ev));
}

JadeOption EudaqWriter::Post(){  
  

  
  static const std::string url_base_class("/JadeWrite/");
  if( ! url.compare(0, url_base_class.size(), url_base_class) ){
    return JadePost::Post(url.substr(url_base_class.size()-1, opt));
  }  
  return "{\"status\":false}";
}


  JadeOption Post(const std::string &url, const JadeOption &opt) override{    
    if(url == "/eudaq_init/"){
      
      return "{\"status\":true}";
    }
    if(url == "/eudaq_config/"){
      
      
      return "{\"status\":true}";
    }
    if(url == "/eudaq_start/"){
      JadeManager::DeviceConnect();
      JadeManager::DeviceControl("START");
      JadeManager::StartDataTaking();
      return "{\"status\":true}";
    }
    if(url == "/eudaq_stop/"){
      JadeManager::StopDataTaking();
      JadeManager::DeviceControl("STOP");
      JadeManager::DeviceDisconnect();  
      return "{\"status\":ture}";
    }
    if(url == "/eudaq_reset/"){
      // JadeManager::reset();
      return "{\"status\":true}";
    }
    if(url == "/eudaq_terminate/"){
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
