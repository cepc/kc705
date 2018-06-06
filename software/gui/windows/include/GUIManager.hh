#ifndef GUIManager_HH
#define GUIManager_HH

#include <QObject>
#include <QDebug>

#include "JadeManager.hh"
#include "JadeRegCtrl.hh"
#include "GUIMonitor.hh"

#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <ctime>

class GUIManager : public QObject 
{
  Q_OBJECT

  public:
    GUIManager();
    ~GUIManager();
    std::string get_now_str();

    void set_input_data_path(std::string input_path){m_opt_data_input = input_path;}; 
    void set_register_data_path(std::string reg_path){m_opt_reg = reg_path;}; 
    void set_output_data_path(std::string output_path){m_opt_data_output = output_path;}; 
    void set_run_time(std::string time_run){m_opt_time_run = time_run;}; 
    void set_ev_print(std::string ev_print){m_opt_ev_print = ev_print;}; 
    void set_chip_address(int chip_address){m_opt_chip_address = chip_address;}; 
    void set_nfiles(int num){m_opt_nfiles = num;};
    void set_channel(int col, int row){m_col = col; m_row = row;};
    void set_adc_threshold(int thr){m_thr = thr;};//*

    std::string get_input_data_path(){return m_opt_data_input;}; 
    std::string get_register_data_path(){return m_opt_reg;}; 
    std::string get_output_data_path(){return m_opt_data_output;}; 
    int get_chip_address(){return m_opt_chip_address;};
    size_t get_run_time(){return std::stoul(m_opt_time_run);}; 
    size_t get_ev_print(){return std::stoul(m_opt_ev_print);}; 
    int get_nfiles(){return m_opt_nfiles;};
    std::shared_ptr<GUIMonitor>get_monitor();
 
  public slots:
    void start_run();
    void stop_run();
    void config();
  
  private:
    std::string m_opt_data_input;
    std::string m_opt_reg;
    std::string m_opt_data_output;
    std::string m_opt_time_run;
    std::string m_opt_ev_print;
    int m_opt_chip_address;
    int m_opt_nfiles;
    int m_col;
    int m_row;
    int m_thr;//*
    JadeManager* m_man;
    std::shared_ptr<GUIMonitor> m_monitor;

  signals:
    void IsRunning();
    void IsStop();
};


#endif //GUIManager_HH
