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
  
  JadeOption conf_man = conf_core.GetSubOption("JadeManager");
  JadeOption conf_man_para = conf_man.GetSubOption("parameter");
  JadeOption conf_ctrl = conf_core.GetSubOption("JadeRegCtrl");
  JadeOption conf_ctrl_para = conf_ctrl.GetSubOption("parameter");
  JadeOption conf_read = conf_core.GetSubOption("JadeRead");
  JadeOption conf_read_para = conf_read.GetSubOption("parameter");
  JadeOption conf_flt = conf_core.GetSubOption("JadeFilter");
  JadeOption conf_flt_para = conf_flt.GetSubOption("parameter");
  JadeOption conf_wrt = conf_core.GetSubOption("JadeWrite");
  JadeOption conf_wrt_para = conf_wrt.GetSubOption("parameter");
  JadeOption conf_mnt = conf_core.GetSubOption("JadeMonitor");
  JadeOption conf_mnt_para = conf_mnt.GetSubOption("parameter");

  std::cout<<conf_man_para.DumpString()<<std::endl;
  std::cout<<conf_ctrl_para.DumpString()<<std::endl;
  std::cout<<conf_read_para.DumpString()<<std::endl;
  std::cout<<conf_flt_para.DumpString()<<std::endl;
  std::cout<<conf_wrt_para.DumpString()<<std::endl;
  std::cout<<conf_mnt_para.DumpString()<<std::endl;
  JadeManager pman(conf_man_para);
  JadeRegCtrlSP pctrl = std::make_shared<JadeRegCtrl>(conf_ctrl_para);
  JadeReadSP pread = std::make_shared<JadeRead>(conf_read_para);
  JadeFilterSP pflt = std::make_shared<JadeFilter>(conf_flt_para);
  JadeWriteSP pwrt = std::make_shared<JadeWrite>(conf_wrt_para);
  JadeMonitorSP pmnt = std::make_shared<JadeMonitor>(conf_mnt_para);
  
  pman.SetRegCtrl(pctrl);
  pman.SetReader(pread);
  pman.SetFilter(pflt);
  pman.SetWriter(pwrt);
  pman.SetMonitor(pmnt);
  pman.DeviceConnect();
  for(int i=0; i< 20; i++){
    std::cout<<"=========start at "<<get_now_str()<<"======="<< std::endl;
    pman.DeviceControl("START");
    pman.DeviceControl("START");
    std::cout<<"========="<<std::endl;
    pman.StartDataTaking(); 
    std::cout<<"========="<<std::endl;      
    std::this_thread::sleep_for(100s);
    std::cout<<"========="<<std::endl;
    pman.StopDataTaking();
    std::cout<<"========="<<std::endl;
    pman.DeviceControl("STOP");
    std::this_thread::sleep_for(1s);
    std::cout<<"=========exit at "<<get_now_str()<<"======="<< std::endl;
  }
  pman.DeviceDisconnect();
  return 0;

}
