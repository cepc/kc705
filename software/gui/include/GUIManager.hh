#ifndef GUIManager_HH
#define GUIManager_HH

#include "JadeManager.hh"
#include "GUIMonitor.hh"
#include <string>
#include <sstream>
#include <iostream>

class GUIManager
{
  public:
    GUIManager();
    ~GUIManager();
    std::string get_now_str();

    void set_input_data_path(std::string input_path){opt_data_input = input_path;}; 
    void set_register_data_path(std::string reg_path){opt_reg = reg_path;}; 
    void set_output_data_path(std::string output_path){opt_data_output = output_path;}; 
    void set_run_time(std::string time_run){opt_time_run = time_run;}; 
    void set_ev_print(std::string ev_print){opt_ev_print = ev_print;}; 
    void set_chip_address(int chip_address){opt_chip_address = chip_address;}; 
    void set_nfiles(int num){opt_nfiles = num;};

    std::string get_input_data_path(){return opt_data_input;}; 
    std::string get_register_data_path(){return opt_reg;}; 
    std::string get_output_data_path(){return opt_data_output;}; 
    int get_chip_address(){return opt_chip_address;};
    size_t get_run_time(){return std::stoul(opt_time_run);}; 
    size_t get_ev_print(){return std::stoul(opt_ev_print);}; 
    int get_nfiles(){return opt_nfiles;};
    std::string get_state();
    int start_run();
    int stop_run();
    void config();
    std::shared_ptr<GUIMonitor>get_monitor();
  
  private:
    std::string opt_data_input;
    std::string opt_reg;
    std::string opt_data_output;
    std::string opt_time_run;
    std::string opt_ev_print;
    int opt_chip_address;
    int opt_nfiles;
    JadeManager* pman;
    std::shared_ptr<GUIMonitor> pmonitor;
};


#endif //GUIManager_HH
