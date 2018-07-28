#ifndef _JADE_EUDAQ_MANAGER_HH
#define _JADE_EUDAQ_MANAGER_HH

#include "JadeCore.hh"

class EudaqManager: public JadeManager{
  using JadeManager::JadeManager;
  JadeOption Post(const std::string &url, const JadeOption &opt) override;  
}

#endif
