#include "JadeMonitor.hh"

JadeMonitor::JadeMonitor(const JadeOption& opt)
  :m_opt(opt),m_ev_n(0), m_ev_print(0){
  m_ev_print = opt.GetIntValue("PRINT_EVENT_N");
}

JadeMonitor::~JadeMonitor(){

}

void JadeMonitor::Monitor(JadeDataFrameSP df){
  if(m_ev_print!=0 && m_ev_n%m_ev_print == 0){
    df->Print(std::cout);
  }
  m_ev_n++;
}
