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
  JadeOption opt_core = opt_conf.GetSubOption("JadeCore");  
  JadeOption opt_man = opt_core.GetSubOption("JadeManager");
  std::string man_type = opt_man.GetStringValue("type");
  JadeOption opt_man_para = opt_man.GetSubOption("parameter");
  std::cout<<opt_man.DumpString()<<std::endl;
  JadeManagerSP pman = JadeManager::Make(man_type, opt_man_para);
  
  
  JadeOption opt_sys_test = opt_conf.GetSubOption("SystemTest");
  size_t nsec = opt_sys_test.GetIntValue("SecPerLoop");
  size_t nloop = opt_sys_test.GetIntValue("N_Loops");
  std::string chip_address = opt_sys_test.GetStringValue("ChipAddress");
  
  // pman->DeviceConnect();
  // pman->DeviceControl(chip_address);
  // pman->DeviceControl("SET");
  // std::cout << "Select address: " << chip_address << std::endl;
  // pman->DeviceDisconnect();

  for(size_t i=0; i< nloop; i++){
    // pman->DeviceConnect();
    // pman->DeviceControl("STOP");
    // std::this_thread::sleep_for(1s);
    std::cout<<"=========start at "<<JadeUtils::GetNowStr()<<"======="<< std::endl;
    // pman->DeviceControl("START");
    // std::cout<<"========="<<std::endl;
    pman->StartDataTaking();
    std::cout<<"========="<<std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(nsec));
    std::cout<<"========="<<std::endl;
    pman->StopDataTaking();
    // std::cout<<"========="<<std::endl;
    // pman->DeviceControl("STOP");
    // std::this_thread::sleep_for(1s);
    std::cout<<"=========exit at "<<JadeUtils::GetNowStr()<<"======="<< std::endl;
    // pman->DeviceDisconnect();
  }
  return 0;
}
