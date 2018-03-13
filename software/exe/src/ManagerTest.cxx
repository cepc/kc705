#include "JadeManager.hh"
#include <iostream>
#include <sstream>
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
  std::cout<<"options: -i <input_data_path> -o <output_data_path> -r <register_path> -s <run_time_milliseconds> -p <print_per_N_events>"<<std::endl;
  
  std::string opt_data_input = "//./xillybus_read_32";
  std::string opt_reg = "//./xillybus_mem_8";
  std::string opt_data_output = "output";
  std::string opt_time_run = "60";
  std::string opt_ev_print = "10000";
  
  for(int i = 1; i < argc; i++){
    std::string opt(argv[i]);
    if(opt=="-i"){
      if(i+1<argc){
	i++;
	opt_data_input = argv[i];
      }
    }
    if(opt=="-o"){ 
      if(i+1<argc){
	i++;
	opt_data_output = argv[i];
      }
    }
    if(opt=="-r"){ 
      if(i+1<argc){
	i++;
	opt_reg = argv[i];
      }
    }
    if(opt=="-s"){ 
      if(i+1<argc){
	i++;
	opt_time_run = argv[i];
      }
    }
    if(opt=="-p"){ 
      if(i+1<argc){
	i++;
	opt_ev_print = argv[i];
      }
    }
  }
  
  size_t time_run = std::stoul(opt_time_run);
  size_t ev_print = std::stoul(opt_ev_print);
  
  std::cout<< "{opt_data_input:"<<opt_data_input<<"}"<<std::endl;
  std::cout<< "{opt_reg:"<<opt_reg<<"}"<<std::endl;
  std::cout<< "{opt_data_output:"<<opt_data_output<<"}"<<std::endl;
  std::cout<< "{time_run:"<<time_run<<"}"<<std::endl;
  std::cout<< "{ev_print:"<<ev_print<<"}"<<std::endl;

  JadeManager pman(JadeOption("{}"));
  pman.SetRegCtrl(std::make_shared<JadeRegCtrl>(JadeOption("{\"PATH\":\""+opt_reg+"\"}")));
  pman.SetReader(std::make_shared<JadeRead>(JadeOption("{\"PATH\":\""+opt_data_input+"\"}")));
  pman.SetFilter(std::make_shared<JadeFilter>(JadeOption("{}")));
  pman.SetWriter(std::make_shared<JadeWrite>(JadeOption("{\"PATH\":\""+opt_data_output+"\"}")));
  pman.SetMonitor(std::make_shared<JadeMonitor>(JadeOption("{\"PRINT_EVENT_N\":"+opt_ev_print+"}")));
  for(int i=0; i< 3; i++){
    std::cout<<"=========start at "<<get_now_str()<<"======="<< std::endl;
    pman.DeviceConnect();
    pman.StartDataTaking(); 
    std::this_thread::sleep_for(std::chrono::milliseconds(time_run));
    pman.StopDataTaking();
    pman.DeviceDisconnect();
    pman.Reset();//remove
    std::cout<<"=========eixt at "<<get_now_str()<<"======="<< std::endl;
  }
  return 0;
}
