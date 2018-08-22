#include "GUIManager.hh"
#include "JadeCore.hh"

using namespace std::chrono_literals;

GUIManager::GUIManager()
  :m_opt_config_file("sample_gui.json")
{
  JadeUtils::PrintTypeIndexMap();
}

GUIManager::~GUIManager()
{

}

void GUIManager::start_run()
{
  m_man->StartDataTaking();
  emit IsRunning();
}

void GUIManager::stop_run()
{
  emit IsStop();
  m_man->StopDataTaking();
  std::this_thread::sleep_for(1s);
}

void GUIManager::config(){
  std::string json_path = m_opt_config_file;
  std::string str_json = JadeUtils::LoadFileToString(json_path);
  JadeOption opt_conf(str_json);
  JadeOption opt_man(opt_conf.GetSubOption("JadeManager"));
  std::string man_type=opt_man.GetStringValue("type");
  
  //if(man_type != "GUIManager"){
  //  std::cerr<<"it is not GUIManager \n";
  //  throw;
  //}
  
  m_man = JadeManager::Make(man_type, opt_man.GetSubOption("parameter"));
  m_man->Init();
}

std::shared_ptr<GUIMonitor> GUIManager::get_monitor(){
  JadeMonitorSP mon = m_man->GetMonitor();
  auto guimon = std::dynamic_pointer_cast<GUIMonitor>(mon);
  mon.reset();
  if( ! guimon ){
    std::cerr<<"GUIManager: WARNING, the returned monitor is not GUI devivation "<<std::endl;
    return nullptr;
  }
  //if( guimon.use_count() > 2){
  //  std::cerr<<"GUIManager: WARNING, guimon.use_count() > 2  "<<std::endl;
  //}
  return guimon;
}
