#include "GUIMonitor.hh"


using namespace std::chrono_literals;

GUIMonitor::GUIMonitor(const JadeOption& options):
  JadeMonitor(options),
  m_opt(options),
  m_ev_get(0),
  m_ev_num(0),
  m_nx(16),
  m_ny(48),
  m_col(0),
  m_row(0),
  m_thr(0)//*
{
  m_ev_get = m_opt.GetIntValue("PRINT_EVENT_N");
  m_curr_time = m_opt.GetStringValue("CURRENT_TIME");
  m_col = m_opt.GetIntValue("COLUMN");
  m_row = m_opt.GetIntValue("ROW");
  m_thr = m_opt.GetIntValue("ADC_THREASHOLD");

  m_last_frame_adc.resize(m_nx*m_ny,0);
  m_sum_frame_adc.resize(m_nx*m_ny,0);
  m_count_frame_adc.resize(m_nx*m_ny,0);//add count map

  m_u_adcFrame = std::make_shared<adcFrame>();

  m_u_adcFrame->cds_frame_adc.resize(m_nx*m_ny,0);
  m_u_adcFrame->mean_frame_adc.resize(m_nx*m_ny,0);
  m_u_adcFrame->rms_frame_adc.resize(m_nx*m_ny,0);

  m_u_adcFrame->hist_mean = {{0,0}};
  m_u_adcFrame->hist_rms = {{0,0}};

  m_adcFrame = m_u_adcFrame; 
}

void GUIMonitor::Monitor(JadeDataFrameSP df)
{
  if( m_ev_get!=0 && (m_ev_num++) % m_ev_get < 10 ){
    df->Print(std::cout);
    std::unique_lock<std::mutex> lk_in(m_mx_set);
    m_df = df; 
    lk_in.unlock();

    auto adc_value = m_df->GetFrameData();

    if( m_ev_num %2==1 ) m_last_frame_adc.assign(adc_value.begin(), adc_value.end()); 

    if( m_ev_num %2==0 ) std::transform(adc_value.begin(), adc_value.end(), m_last_frame_adc.begin(), m_u_adcFrame->cds_frame_adc.begin(), std::minus<int16_t>()); 

    //m_last_frame_adc.swap(adc_value); 

    std::transform(m_sum_frame_adc.begin(), m_sum_frame_adc.end(), m_u_adcFrame->cds_frame_adc.begin(), m_sum_frame_adc.begin(), std::plus<int16_t>());

    std::transform(m_sum_frame_adc.begin(), m_sum_frame_adc.end(), m_u_adcFrame->mean_frame_adc.begin(), std::bind2nd(std::divides<int16_t>(),m_ev_num)); 

    std::transform(m_mean_frame_adc.begin(), m_mean_frame_adc.end(), m_u_adcFrame->cds_frame_adc.begin(), m_u_adcFrame->rms_frame_adc.begin(), std::minus<int16_t>()); 

    //m_mean.push_back(m_u_adcFrame->mean_frame_adc.at(m_row+m_col*m_ny));     
    //m_rms.push_back(m_u_adcFrame->rms_frame_adc.at(m_row+m_col*m_ny));
    m_cds_adc.push_back(m_u_adcFrame->cds_frame_adc.at(m_row+m_col*m_ny));

    std::vector<int16_t> count_frame_adc;
    count_frame_adc.resize(48*16,0); 
    std::transform(m_u_adcFrame->cds_frame_adc.begin(),m_u_adcFrame->cds_frame_adc.end(), count_frame_adc.begin(),[=](auto& cds){return cds>m_thr ? 1:0;});

    std::transform(m_count_frame_adc.begin(),m_count_frame_adc.end(),count_frame_adc.begin(),m_count_frame_adc.begin(),std::plus<int16_t>());
    count_frame_adc.clear();
    //GetHistogram(m_mean, m_u_adcFrame->hist_mean);
    //GetHistogram(m_rms, m_u_adcFrame->hist_rms);
    GetHistogram(m_cds_adc, m_u_adcFrame->hist_cds_adc);

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
      m_adc_map->setCell(iy, ix, u_adcFrame->cds_frame_adc.at(iy+m_ny*ix));
      //std::cout << u_adcFrame->cds_frame_adc.at(iy+m_ny*ix) << " ";
    }
    //std::cout << "\n";
  }
  //std::cout << "\nend send" << std::endl;

  return m_adc_map;
}


QVector<QCPGraphData> GUIMonitor::GetHistADC(int col, int row){

  std::cout << "Get Hist ADC..."<<std::endl;

  std::unique_lock<std::mutex> lk_in(m_mx_get);
  auto u_adcFrame = m_adcFrame; 
  lk_in.unlock();

  QCPGraphData point;
  
  for(auto adcItr : u_adcFrame->hist_cds_adc){
    point.key = adcItr.first; 
    point.value = adcItr.second;
    m_hist_cds_adc.append(point);
  }

  return m_hist_cds_adc;
}

QCPColorMapData* GUIMonitor::GetCountMap()
{
  // std::unique_lock<std::mutex> lk_in(m_mx_get);
  // auto u_adcFrame = m_adcFrame; 
  // lk_in.unlock();

  m_count_map = new QCPColorMapData(m_ny, m_nx, QCPRange(0,m_ny), QCPRange(0,m_nx));

  for (size_t iy = 0; iy < m_ny; iy++){
    for (size_t ix = 0; ix < m_nx; ix++)
    {
      m_count_map->setCell(iy, ix, m_count_frame_adc.at(iy+m_ny*ix));
      //std::cout << m_count_frame_adc.at(iy+m_ny*ix) << " ";
    }
    //std::cout << "\n";
  }
  //std::cout << "\nend send" << std::endl;

  return m_count_map;
}




// QVector<QCPGraphData> GUIMonitor::GetPedestal(int col, int row){

//   std::cout << "Get Pedestal..."<<std::endl;

//   std::unique_lock<std::mutex> lk_in(m_mx_get);
//   auto u_adcFrame = m_adcFrame; 
//   lk_in.unlock();

//   QCPGraphData point;

//   for(auto meanItr : u_adcFrame->hist_mean){
//     point.key = meanItr.first; 
//     point.value = meanItr.second;
//     m_pedestal.append(point);
//   }

//   return m_pedestal;
// }

// QVector<QCPGraphData> GUIMonitor::GetNoise(int col, int row){

//   std::cout << "Get Noise..."<<std::endl;

//   std::unique_lock<std::mutex> lk_in(m_mx_get);
//   auto u_adcFrame = m_adcFrame; 
//   lk_in.unlock();

//   QCPGraphData point;

//   for(auto rmsItr : u_adcFrame->hist_rms){
//     point.key = rmsItr.first; 
//     point.value = rmsItr.second;
//     m_noise.append(point);
//   }

//   return m_noise;
// }

void GUIMonitor::Reset(){
  m_u_adcFrame->hist_mean.clear();
  m_u_adcFrame->hist_rms.clear();
}


void GUIMonitor::GetHistogram(const std::vector<double>& xVec, std::map<double, size_t>& hMap) {

  size_t bmax=0;

  std::for_each(xVec.begin(), xVec.end(), 
      [&hMap, &bmax](const double &element)
      {
      hMap[element]++;
      bmax=std::max(bmax, hMap[element]);
      });
}


void GUIMonitor::GetHistogram(const std::vector<int16_t>& xVec, std::map<int16_t,size_t>& hMap) {

  size_t bmax=0;

  std::for_each(xVec.begin(), xVec.end(), 
      [&hMap, &bmax](const int16_t &element)
      {
      hMap[element]++;
      bmax=std::max(bmax, hMap[element]);
      });

}   