#include "JadeManager.hh"
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <ctime>

using namespace std::chrono_literals;

std::string get_now_str(){
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss<<std::put_time(std::localtime(&now_c), "%c");
    return ss.str();
}

int main(int argc, char **argv){
  std::cout<<"options: -c <config_file>"<<std::endl;
  std::string config_file = "sample.json";
  
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
  
  std::ifstream ifs(config_file);
  if(!ifs.good()){
    std::cerr<<"Unable to open configrue file in PATH: "<<config_file<<"\n";
    throw;
  }
  
  std::string config_str;
  config_str.assign((std::istreambuf_iterator<char>(ifs) ),
                (std::istreambuf_iterator<char>()));

  JadeOption conf(config_str);
  JadeOption conf_core = conf.GetSubOption("JadeCore");
  
  JadeOption conf_ctrl = conf_core.GetSubOption("JadeRegCtrl");
  JadeOption conf_ctrl_para = conf_ctrl.GetSubOption("parameter");
  JadeOption conf_read = conf_core.GetSubOption("JadeRead");
  JadeOption conf_read_para = conf_read.GetSubOption("parameter");

  JadeRead rd(conf_read_para);
  rd.Open();
  JadeRegCtrl ctrl(conf_ctrl_para);
  ctrl.Open();

  ctrl.SendCommand("START");
  while(1){
    auto df_v = rd.Read(1, 10ms);
    df_v.clear();
  }
  
}
