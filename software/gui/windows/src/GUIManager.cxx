#include "GUIManager.hh"

using namespace std::chrono_literals;

GUIManager::GUIManager():m_opt_data_input("//./xillybus_read_32"),m_opt_reg("//./xillybus_mem_8"),m_opt_time_run("60"),m_opt_ev_print("10000"),m_opt_chip_address(1),m_opt_nfiles(1),m_col(0),m_row(0){
  m_man = new JadeManager(JadeOption("{}"));
}

GUIManager::~GUIManager(){
}

std::string GUIManager::get_now_str(){
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss<<std::put_time(std::localtime(&now_c), "%c");
    return ss.str();
}

void GUIManager::start_run(){
  std::cout<<"=========start at "<<get_now_str()<<"======="<< std::endl; 
  m_man->DeviceConnect();
  m_man->DeviceControl("STOP");
  std::this_thread::sleep_for(1s);
  m_man->StartDataTaking(); 
  emit IsRunning();
}

void GUIManager::stop_run(){
  emit IsStop();
  m_man->StopDataTaking();
  m_man->DeviceControl("STOP");
  std::this_thread::sleep_for(1s);
  m_man->DeviceDisconnect();
  std::cout<<"=========exit at "<<get_now_str()<<"======="<< std::endl; 
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
  std::string data_output_path = m_opt_data_output+"_"+time_str +".df";

  std::cout<< "{opt_data_input:"<<m_opt_data_input<<"}"<<std::endl;
  std::cout<< "{opt_reg:"<<m_opt_reg<<"}"<<std::endl;
  std::cout<< "{data_output_path:"<<data_output_path<<"}"<<std::endl;
  std::cout<< "{time_run:"<<time_run<<"}"<<std::endl;
  std::cout<< "{ev_print:"<<ev_print<<"}"<<std::endl;
  std::cout<< "{chip adress: CHIPA"<<m_opt_chip_address<<"}"<<std::endl;

  m_man->SetRegCtrl(std::make_shared<JadeRegCtrl>(JadeOption("{\"PATH\":\""+m_opt_reg+"\"}")));
  m_man->SetReader(std::make_shared<JadeRead>(JadeOption("{\"PATH\":\""+m_opt_data_input+"\"}")));
  m_man->SetFilter(std::make_shared<JadeFilter>(JadeOption("{}")));
  m_man->SetWriter(std::make_shared<JadeWrite>(JadeOption("{\"PATH\":\""+data_output_path+"\"}")));
  m_monitor = std::make_shared<GUIMonitor>(JadeOption("{\"PRINT_EVENT_N\":"+m_opt_ev_print+",\"CURRENT_TIME\":\""+time_str+"\",\"COLUMN\":"+std::to_string(m_col)+",\"ROW\":"+std::to_string(m_row)+",\"ADC_THREASHOLD\":"+std::to_string(m_thr)+"}"));
  m_man->SetMonitor(std::dynamic_pointer_cast<JadeMonitor>(m_monitor));

  std::string cmd = "CHIPA" + std::to_string(m_opt_chip_address);
  m_man->DeviceConnect();
  m_man->DeviceControl(cmd);
  m_man->DeviceControl("SET");
  std::this_thread::sleep_for(200ms);
  m_man->DeviceDisconnect();
}

std::shared_ptr<GUIMonitor>GUIManager::get_monitor(){
  return m_monitor; 
}
