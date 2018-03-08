#ifndef MONITOR_HH
#define MONITOR_HH

#include "JadeManager.hh"
#include <string>
#include <sstream>
#include <iostream>

class Monitor
{
  public:
    Monitor();
    ~Monitor();
    std::string get_now_str();

    void set_input_data_path(std::string input_path){opt_data_input = input_path;}; 
    void set_register_data_path(std::string reg_path){opt_reg = reg_path;}; 
    void set_output_data_path(std::string output_path){opt_data_output = output_path;}; 
    void set_run_time(std::string time_run){opt_time_run = time_run;}; 
    void set_ev_print(std::string ev_print){opt_ev_print = ev_print;}; 

    std::string get_input_data_path(){return opt_data_input;}; 
    std::string get_register_data_path(){return opt_reg;}; 
    std::string get_output_data_path(){return opt_data_output;}; 
    size_t get_run_time(){return std::stoul(opt_time_run);}; 
    size_t get_ev_print(){return std::stoul(opt_ev_print);}; 

    int start_run();
    int stop_run();

  private:
    std::string opt_data_input;
    std::string opt_reg;
    std::string opt_data_output;
    std::string opt_time_run;
    std::string opt_ev_print;
    JadeManager* pman;
};

#endif //MONITOR_HH
