#include "JadeCore.hh"
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <ctime>

using namespace std::chrono_literals;

int main(int argc, char **argv){
  
  std::cout<<"options: -c <config_file>"<<std::endl;
  std::string config_file = "sample_summer.json";
  
  for(int i = 1; i < argc; i++){
    std::string opt(argv[i]);
    if(opt=="-c"){ 
      if(i+1<argc){
	i++;
	config_file = argv[i];
      }
    }
  }
  
  size_t found = config_file.find_last_of(".");
  if(found == std::string::npos  ||
     found+1 >= config_file.size() ||
     config_file.substr(found+1) != "json"){
    std::cerr<<"Please provide configure file with json suffix\n";
    throw;
  }
  
  std::string config_str = JadeUtils::LoadFileToString(config_file);
  
  JadeOption opt_conf(config_str);
  JadeOption opt_man = opt_conf.GetSubOption("JadeManager");
  std::cout<<opt_man.DumpString()<<std::endl;
  std::string man_type = opt_man.GetStringValue("type");
  JadeOption opt_man_para = opt_man.GetSubOption("parameter");
  JadeManagerSP pman = JadeManager::Make(man_type, opt_man_para);
    
  JadeOption opt_sys_test = opt_conf.GetSubOption("SystemTest");
  size_t nsec = opt_sys_test.GetIntValue("SecPerLoop");
  size_t nloop = opt_sys_test.GetIntValue("N_Loops");
  
  for(size_t i=0; i< nloop; i++){
    std::cout<<"=========start at "<<JadeUtils::GetNowStr()<<"======="<< std::endl;
    pman->StartDataTaking();
    std::cout<<"========="<<std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(nsec));
    std::cout<<"========="<<std::endl;
    pman->StopDataTaking();
    std::cout<<"=========exit at "<<JadeUtils::GetNowStr()<<"======="<< std::endl;
  }
  return 0;
}
