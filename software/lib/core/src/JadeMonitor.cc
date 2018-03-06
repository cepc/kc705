#include "JadeMonitor.hh"

JadeMonitor::JadeMonitor(const std::string& options)
  :m_options(options),m_ev_n(0),m_ev_print(0){
  if(!options.empty())
    m_ev_print = std::stoul(options);
}

void JadeMonitor::Monitor(JadeDataFrameSP df){
  if(m_ev_print!=0 && m_ev_n%m_ev_print == 0){
    df->Print(std::cout);
  }
  m_ev_n++;
}
