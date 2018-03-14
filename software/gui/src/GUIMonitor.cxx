#include "GUIMonitor.hh"
#include <iostream>


using namespace std::chrono_literals;

GUIMonitor::GUIMonitor(const JadeOption& options):
  JadeMonitor(options),
  m_opt(options),
  m_ev_get(0),
  m_ev_num(0),
  m_nx(16),
  m_ny(48)
{
  m_ev_get = m_opt.GetIntValue("PRINT_EVENT_N");
  m_curr_time = m_opt.GetStringValue("CURRENT_TIME");

  m_last_frame_adc.resize(m_nx*m_ny,0);
  m_cds_frame_adc.resize(m_nx*m_ny,0);
  m_sum_frame_adc.resize(m_nx*m_ny,0);
  m_mean_frame_adc.resize(m_nx*m_ny,0);
  m_rms_frame_adc.resize(m_nx*m_ny,0);

  m_adcFrame.cds_frame_adc.swap(m_cds_frame_adc);
  m_adcFrame.mean_frame_adc.swap(m_mean_frame_adc);
  m_adcFrame.rms_frame_adc.swap(m_rms_frame_adc);

  m_hist_mean = std::shared_ptr<TH1D>(new TH1D(Form("mean_%s",m_curr_time),"mean",4000,-2000,2000));
  m_hist_rms = std::shared_ptr<TH1D>(new TH1D(Form("rms_%s",m_curr_time),"rms",4000,-2000,2000));
}

void GUIMonitor::Monitor(JadeDataFrameSP df)
{
  if( !m_ev_get && (m_ev_num++) % m_ev_get == 0 ){
  std::unique_lock<std::mutex> lk_in(m_mx_set);
  m_df = df; 
  lk_in.unlock();

  auto adc_value = m_df->GetFrameData();

  if(m_ev_num==1) m_last_frame_adc.assign(adc_value.begin(), adc_value.end()); 

  std::transform(adc_value.begin(), adc_value.end(), m_last_frame_adc.begin(), m_cds_frame_adc.begin(), std::minus<int16_t>()); 

  m_last_frame_adc.swap(adc_value); 

  std::transform(m_sum_frame_adc.begin(), m_sum_frame_adc.end(), m_cds_frame_adc.begin(), m_sum_frame_adc.begin(), std::plus<int16_t>()); 

  std::transform(m_sum_frame_adc.begin(), m_sum_frame_adc.end(), m_mean_frame_adc.begin(), std::bind2nd(std::divides<int16_t>(),m_ev_num)); 

  std::transform(m_mean_frame_adc.begin(), m_mean_frame_adc.end(), m_cds_frame_adc.begin(), m_rms_frame_adc.begin(), std::minus<int16_t>()); 
  
  m_u_adcFrame.cds_frame_adc.swap(m_cds_frame_adc);
  m_u_adcFrame.mean_frame_adc.swap(m_mean_frame_adc);
  m_u_adcFrame.rms_frame_adc.swap(m_rms_frame_adc);
  
  std::unique_lock<std::mutex> lk_out(m_mx_get);
  m_adcFrame = m_u_adcFrame; 
  lk_out.unlock();
  }
}

QCPColorMapData* GUIMonitor::GetADCMap()
{
  std::unique_lock<std::mutex> lk_in(m_mx_get);
  auto u_adcFrame = m_adcFrame; 
  lk_in.unlock();

  m_adc_map = new QCPColorMapData(m_ny, m_nx, QCPRange(0,m_ny), QCPRange(0,m_nx));

  for (size_t iy = 0; iy < m_ny; iy++){
    for (size_t ix = 0; ix < m_nx; ix++)
    {
      m_adc_map->setCell(iy, ix, u_adcFrame.cds_frame_adc.at(iy+m_ny*ix));
      std::cout << u_adcFrame.cds_frame_adc.at(iy+m_ny*ix) << " ";
    }
    std::cout << "\n";
  }
  std::cout << "\nend send" << std::endl;

  return m_adc_map;
}

QVector<QCPGraphData> GUIMonitor::GetPedestal(int col, int row){

  std::cout << "Get Pedestal..."<<std::endl;
  
  std::unique_lock<std::mutex> lk_in(m_mx_get);
  auto u_adcFrame = m_adcFrame; 
  lk_in.unlock();
  
  m_hist_mean->Fill(u_adcFrame.mean_frame_adc.at(row+col*m_ny));
  
  QCPGraphData point;

  for(int iBin=0; iBin<m_hist_mean->GetNbinsX(); iBin++){
    point.key = m_hist_mean->GetXaxis()->GetBinCenter(iBin); 
    point.value = m_hist_mean->GetBinContent(iBin);
    m_pedestal.append(point);
  }
  return m_pedestal;
}

QVector<QCPGraphData> GUIMonitor::GetNoise(int col, int row){

  std::cout << "Get Noise..."<<std::endl;

  std::unique_lock<std::mutex> lk_in(m_mx_get);
  auto u_adcFrame = m_adcFrame; 
  lk_in.unlock();
  
  m_hist_rms->Fill(u_adcFrame.rms_frame_adc.at(row+col*m_ny));

  QCPGraphData point;

  for(int iBin=0; iBin<m_hist_rms->GetNbinsX(); iBin++){
    point.key = m_hist_rms->GetXaxis()->GetBinCenter(iBin); 
    point.value = m_hist_rms->GetBinContent(iBin);
    m_noise.append(point);
  }

  return m_noise;
}

void GUIMonitor::Reset(){
  m_hist_mean->Reset();
  m_hist_rms->Reset();
}
