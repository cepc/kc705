#include "JadeCore.hh"

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

  //ctrl.SendCommand("STOP");
  //std::this_thread::sleep_for(500ms);
  ctrl.SendCommand("START");

  auto tp_start = std::chrono::system_clock::now();
  auto tp_print_prev = std::chrono::system_clock::now();
  uint64_t ndf = 0;
  uint64_t ndf_print_next = 10000;
  uint64_t ndf_print_prev = 0;
  while(1){
    auto df_v = rd.Read(1, 10ms);
    ndf += df_v.size();
    for(auto&e: df_v){
      //e->Decode();
    }
    df_v.clear();
    if(ndf >= ndf_print_next){
      auto tp_now = std::chrono::system_clock::now();
      auto dur = tp_now - tp_start;
      double dur_sec_c = dur.count() * decltype(dur)::period::num * 1.0/ decltype(dur)::period::den;
      double av_hz = ndf/dur_sec_c;
      dur = tp_now - tp_print_prev;
      dur_sec_c = dur.count() * decltype(dur)::period::num * 1.0/ decltype(dur)::period::den; 
      double cr_hz = (ndf-ndf_print_prev) / dur_sec_c;
      std::cout<<"average data rate: "<< av_hz<<"  curent data rate: "<< cr_hz<<std::endl;
      ndf_print_next += 10000;
      ndf_print_prev = ndf;
      tp_print_prev = tp_now;
    }
  }
  
}
