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
  for(int i=0; i<16; i++)
    for(int j=0; j<48; j++)
    {
      m_last_frame_adc[i][j] = 0;
      m_sum_frame_adc[i][j] = 0;
      m_mean_adc[i][j] = 0;
      m_rms_adc[i][j] = 0;
    }
  m_hist_mean = new TH1D("mean","mean",4000,-2000,2000);
  m_hist_rms = new TH1D("rms","rms",4000,-2000,2000);
}

void GUIMonitor::Monitor(JadeDataFrameSP df)
{
  std::unique_lock<std::mutex> lk_in(m_mx_set);
  m_df = df; 
  lk_in.unlock();
}


void GUIMonitor::ProcessData()
{
  std::unique_lock<std::mutex> lk_out(m_mx_get);
  m_u_df = m_df;  
  lk_out.unlock();

  m_ev_num++;

  m_offset_x = m_u_df->GetMatrixLowX();
  m_offset_y = m_u_df->GetMatrixLowY();

  m_nx = m_u_df->GetMatrixSizeX();
  m_ny = m_u_df->GetMatrixSizeY();

  for (size_t iy = 0; iy < m_ny; iy++)
    for (size_t ix = 0; ix < m_nx; ix++)
    {
      uint16_t adc_value = m_u_df->GetHitValue(ix+m_offset_x, iy+m_offset_y);

      m_cds_frame_adc[ix][iy] = adc_value - m_last_frame_adc[ix][iy]; 

      m_last_frame_adc[ix][iy] = adc_value; 

      m_sum_frame_adc[ix][iy] += m_cds_frame_adc[ix][iy];

      m_mean_adc[ix][iy] = m_sum_frame_adc[ix][iy] / m_ev_num;  

      m_rms_adc[ix][iy] = m_mean_adc[ix][iy] - m_cds_frame_adc[ix][iy];

      std::cout << adc_value << " "; 
    }
}

QCPColorMapData* GUIMonitor::GetADCMap()
{
  m_adc_map = new QCPColorMapData(m_ny, m_nx, QCPRange(0,m_ny), QCPRange(0,m_nx));

  for (size_t iy = 0; iy < m_ny; iy++)
    for (size_t ix = 0; ix < m_nx; ix++)
    {
      m_adc_map->setCell(iy, ix, m_cds_frame_adc[ix][iy]);
    }
  std::cout << "\nend send" << std::endl;

  return m_adc_map;
}

QVector<QCPGraphData> GUIMonitor::GetPedestal(int col, int row){

  std::cout << "Get Pedestal..."<<std::endl;
  
  m_hist_mean->Fill(m_mean_adc[col][row]);
  QCPGraphData point;

  for(int iBin=0; iBin<m_hist_rms->GetNbinsX(); iBin++){
    point.key = m_hist_mean->GetXaxis()->GetBinCenter(iBin); 
    point.value = m_hist_mean->GetBinContent(iBin);
    m_pedestal.append(point);
  }
  return m_pedestal;
}

QVector<QCPGraphData> GUIMonitor::GetNoise(int col, int row){

  std::cout << "Get Noise..."<<std::endl;

  m_hist_rms->Fill(m_rms_adc[col][row]);
  QCPGraphData point;

  for(int iBin=0; iBin<m_hist_rms->GetNbinsX(); iBin++){
    point.key = m_hist_rms->GetXaxis()->GetBinCenter(iBin); 
    point.value = m_hist_rms->GetBinContent(iBin);
    m_noise.append(point);
  }

  return m_noise;
}
