#include "JadeRegCtrl.hh"
#include "monitor.hh"
#include <iomanip>
#include <chrono>
#include <thread>
#include <ctime>

using namespace std::chrono_literals;

Monitor::Monitor():opt_data_input("//./xillybus_read_32"),opt_reg("//./xillybus_mem_8"),opt_time_run("60"),opt_ev_print("10000"){
  pman = new JadeManager();
}

Monitor::~Monitor(){
  delete pman;
}

std::string Monitor::get_now_str(){
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss<<std::put_time(std::localtime(&now_c), "%c");
    return ss.str();
}

int Monitor::start_run(){
  
  size_t time_run = get_run_time();
  size_t ev_print = get_ev_print();
  
  std::time_t time_now = std::time(nullptr);
  char time_buff[13];
  time_buff[12] = 0;
  std::strftime(time_buff, sizeof(time_buff),
		"%y%m%d%H%M%S", std::localtime(&time_now));
  std::string time_str(time_buff);
  std::string data_output_path = opt_data_output+"_"+time_str +".df";

  std::cout<< "{opt_data_input:"<<opt_data_input<<"}"<<std::endl;
  std::cout<< "{opt_reg:"<<opt_reg<<"}"<<std::endl;
  std::cout<< "{data_output_path:"<<data_output_path<<"}"<<std::endl;
  std::cout<< "{time_run:"<<time_run<<"}"<<std::endl;
  std::cout<< "{ev_print:"<<ev_print<<"}"<<std::endl;

  pman->SetRegCtrl(std::make_shared<JadeRegCtrl>(opt_reg));
  pman->SetReader(std::make_shared<JadeRead>(opt_data_input, ""));
  pman->SetFilter(std::make_shared<JadeFilter>(""));
  pman->SetWriter(std::make_shared<JadeWrite>(data_output_path, ""));
  pman->SetMonitor(std::make_shared<JadeMonitor>(std::to_string(ev_print)));
  std::this_thread::sleep_for(200ms);
  std::cout<<"=========start at "<<get_now_str()<<"======="<< std::endl; 
  pman->StartDataTaking(); 
  std::this_thread::sleep_for(std::chrono::milliseconds(time_run));
  pman->StopDataTaking();
  std::cout<<"=========exit at "<<get_now_str()<<"======="<< std::endl; 
  return 0;
}

int Monitor::stop_run(){
  pman->StopDataTaking();
  std::cout<<"=========exit at "<<get_now_str()<<"======="<< std::endl; 
  return 0;
}
