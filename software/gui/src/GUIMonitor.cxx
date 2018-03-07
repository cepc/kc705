#include "GUIMonitor.hh"
#include <iostream>

using namespace std::chrono_literals;

GUIMonitor::GUIMonitor(const std::string& options):JadeMonitor(options),m_opt(options),m_ev_get(0),m_ev_num(0)
{
  if(!options.empty())
    m_ev_get = std::stoul(options);
};

void GUIMonitor::Monitor(JadeDataFrameSP df)
{
  if(m_ev_get!=0 && m_ev_num%m_ev_get == 0){
    GetData(df);
  }
  m_ev_num++;
}

void GUIMonitor::GetData(JadeDataFrameSP df)
{
  uint32_t offset_x = df->GetMatrixLowX();
  uint32_t offset_y = df->GetMatrixLowY();

  uint32_t n_x = df->GetMatrixSizeX();
  uint32_t n_y = df->GetMatrixSizeY();
  
  uint16_t value;
  
  std::unique_lock<std::mutex> lk_in(m_mx_get);
  for(size_t iy=0; iy<n_y; iy++)
    for(size_t ix=0; ix<n_x; ix++)
    {
      uint16_t value = df->GetHitValue(ix+offset_x, iy+offset_y);
      m_qu_data.push(value);
    }
  lk_in.unlock();
  m_cv_data.notify_all();
}

void GUIMonitor::SendData()
{
  std::unique_lock<std::mutex> lk_out(m_mx_send);
  while(m_qu_data.empty()){
    while(m_cv_data.wait_for(lk_out, 10ms)==std::cv_status::timeout){
      return;
    }
  }
  for(int i=0; i<48*16; i++){
    std::cout << m_qu_data.front() << std::endl;
    m_qu_data.pop();
  }
  lk_out.unlock();
}
