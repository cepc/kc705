#include "JadeRegCtrl.hh"
#include "GUIManager.hh"
#include <iomanip>
#include <chrono>
#include <thread>
#include <ctime>

using namespace std::chrono_literals;

GUIManager::GUIManager():opt_data_input("//./xillybus_read_32"),opt_reg("//./xillybus_mem_8"),opt_time_run("60"),opt_ev_print("10000"),opt_chip_address(1),opt_nfiles(1){
  pman = new JadeManager(JadeOption("{}"));
}

GUIManager::~GUIManager(){
  delete pman;
}

std::string GUIManager::get_now_str(){
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss<<std::put_time(std::localtime(&now_c), "%c");
    return ss.str();
}

int GUIManager::start_run(){
  size_t time_run = get_run_time();
  
  std::cout<<"=========start at "<<get_now_str()<<"======="<< std::endl; 
  pman->DeviceConnect();
  pman->StartDataTaking(); 
  std::this_thread::sleep_for(std::chrono::milliseconds(time_run));
  pman->StopDataTaking();
  std::cout<<"=========exit at "<<get_now_str()<<"======="<< std::endl; 
  return 0;
}

int GUIManager::stop_run(){
  pman->StopDataTaking();
  pman->DeviceDisconnect();
  pman->Reset();
  std::cout<<"=========exit at "<<get_now_str()<<"======="<< std::endl; 
  return 0;
}

void GUIManager::config(){
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
  std::cout<< "{chip adress: CHIPA"<<opt_chip_address<<"}"<<std::endl;

  pman->SetRegCtrl(std::make_shared<JadeRegCtrl>(JadeOption("{\"PATH\":\""+opt_reg+"\"}")));
  pman->SetReader(std::make_shared<JadeRead>(JadeOption("{\"PATH\":\""+opt_data_input+"\"}")));
  pman->SetFilter(std::make_shared<JadeFilter>(JadeOption("{}")));
  pman->SetWriter(std::make_shared<JadeWrite>(JadeOption("{\"PATH\":\""+data_output_path+"\"}")));
  pmonitor = std::make_shared<GUIMonitor>(JadeOption("{\"PRINT_EVENT_N\":"+opt_ev_print+"}"));
  pman->SetMonitor(std::static_pointer_cast<JadeMonitor>(pmonitor));

  std::string cmd = "CHIPA" + std::to_string(opt_chip_address);
  pman->DeviceControl(cmd);
  pman->DeviceControl("SET");
  std::this_thread::sleep_for(200ms);
}

//std::string GUIManager::get_state(){
//  if(!pman->DeviceStatus("START")){ 
//    return "START";
//  }else if(!pman->DeviceStatus("STOP")){ 
//    return "STOP";
//  }else{
//    return "ERROR";
//  }
//}

std::shared_ptr<GUIMonitor>GUIManager::get_monitor(){
  return pmonitor; 
}
