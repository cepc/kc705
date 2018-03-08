#include "GUIMonitor.hh"
#include <iostream>

using namespace std::chrono_literals;

GUIMonitor::GUIMonitor(const JadeOption& options):
  JadeMonitor(options),
  m_opt(options),
  m_ev_get(0),
  m_ev_num(0)
{
  m_ev_get = m_opt.GetIntValue("PRINT_EVENT_N");
};

void GUIMonitor::Monitor(JadeDataFrameSP df)
{
  if(m_ev_get!=0 && m_ev_num%m_ev_get == 0){
    SetData(df);
  }
  m_ev_num++;
}

void GUIMonitor::SetData(JadeDataFrameSP df)
{
  m_offset_x = df->GetMatrixLowX();
  m_offset_y = df->GetMatrixLowY();

  m_nx = df->GetMatrixSizeX();
  m_ny = df->GetMatrixSizeY();

  uint16_t value;
  std::unique_lock<std::mutex> lk_in(m_mx_set);
  m_data.clear();
  for(size_t iy=0; iy<m_ny; iy++)
    for(size_t ix=0; ix<m_nx; ix++)
    {
      uint16_t value = df->GetHitValue(ix+m_offset_x, iy+m_offset_y);
      m_data.push_back(value);
    }
  m_qu_data.push(m_data);
  lk_in.unlock();
  m_cv_data.notify_all();
}

QCPColorMapData* GUIMonitor::GetData()
{
  std::unique_lock<std::mutex> lk_out(m_mx_get);
  while(m_data.empty()){
    while(m_cv_data.wait_for(lk_out, 10ms)==std::cv_status::timeout){
      //throw;
      break;
    }
  }

  data = new QCPColorMapData(m_ny, m_nx, QCPRange(0,m_ny), QCPRange(0,m_nx));
  std::vector<uint16_t> u_data = m_qu_data.front();
  if(u_data.size() != 768){
    std::cerr << " Unable to send full frame! " << std::endl;
    std::cerr << " size: " << u_data.size() << std::endl;
  }

  for (unsigned int i = 0; i < m_ny; ++i)
    for (unsigned int j = 0; j < m_nx; ++j)
    {
      size_t pos = j + m_nx*i;
      data->setCell(i, j, u_data.at(pos));
      std::cout << u_data.at(pos) << " "; 
    }
  std::cout << "\nend send" << std::endl;
  u_data.clear();
  m_qu_data.pop();
  lk_out.unlock();

  return data;
}

