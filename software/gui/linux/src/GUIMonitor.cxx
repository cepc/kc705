#include "GUIMonitor.hh"

namespace{
  auto _test_index_ = JadeUtils::SetTypeIndex(std::type_index(typeid(GUIMonitor)));
  auto _test_ = JadeFactory<JadeMonitor>::Register<GUIMonitor, const JadeOption&>(typeid(GUIMonitor));
}

using namespace std::chrono_literals;

GUIMonitor::GUIMonitor(const JadeOption& options)
    : JadeMonitor(options)
    , m_opt(options)
    , m_ev_get(0)
    , m_ev_num(0)
    , m_nx(16)
    , m_ny(48)
    , m_col(0)
    , m_row(0)
    , m_nbins(10000)
    , m_thr(0) //*
    , m_last_df()
    , m_enbale_print_events(false)
    , m_monitor_percent(0.5)
{
  //
  m_ev_get = m_opt.GetIntValue("PRINT_EVENT_N");
  m_enbale_print_events = m_opt.GetIntValue("ENABLE_PRINT_EVENTS");
  m_monitor_percent = m_opt.GetFloatValue("MONITOR_PERCENT");
  m_col = m_opt.GetIntValue("COLUMN");
  m_row = m_opt.GetIntValue("ROW");
  m_thr = m_opt.GetIntValue("ADC_THREASHOLD");
  //
  
  m_nbins = m_opt.GetIntValue("NBINS");
  m_adc_counts = std::shared_ptr<TH2F>(new TH2F("ADC_counts", "ADC_counts", m_nx, 0, m_nx, m_ny, 0, m_ny));
  m_adc_map = std::shared_ptr<TH2F>(new TH2F("ADC_map", "ADC_map", m_nx, 0, m_nx, m_ny, 0, m_ny));
  for (int i = 0; i < m_nx; i++) {
    m_adc_hist.push_back(std::shared_ptr<TH1F>(new TH1F(Form("ADC_COL_%i", i), Form("ADC_COL_%i", i), 2 * m_nbins, -1 * m_nbins, m_nbins)));
  }
}

void GUIMonitor::Reset(){
  m_ev_num = 0;
};

void GUIMonitor::Monitor(JadeDataFrameSP df)
{
  // if (m_ev_num == 0) {
  //   m_last_df = df;
  // } else {
  //   df->CDS(*m_last_df);
  //   m_last_df = df;
  // }

  if (m_enbale_print_events && m_ev_get != 0 && m_ev_num % m_ev_get == 0) {
    df->Print(std::cout);
    // df->PrintCDS(std::cout);
  }

  m_df = df;

  if ((m_ev_num) == 0) {
    m_ev_num++;
    return;
  }

  // if (!m_df->GetCDSStatus()) {
  //   return;
  // }

  TRandom rdm;
  auto factor = rdm.Uniform(1);
  if ((m_ev_num % m_ev_get) < (m_ev_get * factor * m_monitor_percent)) {

    m_adc_map->Reset();
    for (size_t iy = 0; iy < m_ny; iy++)
      for (size_t ix = 0; ix < m_nx; ix++) {
        // auto value = m_df->GetCDSValue(ix, iy);
        auto value = m_df->GetHitValue(ix, iy);
        m_adc_map->SetBinContent(m_nx - ix, m_ny - iy, value);
        if (std::abs(value) > m_thr) {
          m_adc_counts->Fill(m_nx - ix, m_ny - iy);
        }
        if (m_ny - iy == m_col) {
          m_adc_hist[m_nx - ix - 1]->Fill(value);
        }
      }

    std::unique_lock<std::mutex> lk_out(m_mx_get);
    m_adc_counts_clone = m_adc_counts;
    m_adc_map_clone = m_adc_map;
    m_adc_hist_clone = m_adc_hist;
    lk_out.unlock();
  }
  m_ev_num++;
}

std::shared_ptr<TH2F> GUIMonitor::GetADCMap()
{
  return m_adc_map_clone;
}

std::shared_ptr<TH2F> GUIMonitor::GetADCCounts()
{
  return m_adc_counts_clone;
}

std::vector<std::shared_ptr<TH1F>> GUIMonitor::GetADCHist()
{
  return m_adc_hist_clone;
}

JadeOption GUIMonitor::Post(const std::string &url, const JadeOption &opt){
  if(url == "/set_print_event_n"){
    m_ev_get = opt.GetIntValue("PRINT_EVENT_N");
    return "{\"status\":true}";
  }

  if(url == "/set_adc_threshold"){
    m_thr = opt.GetIntValue("ADC_THRESHOLD");
    return "{\"status\":true}";
  }
  
  if(url == "/set_row"){
    m_row = m_opt.GetIntValue("ROW"); 
    return "{\"status\":true}";
  }

  if(url == "/set_column"){
    m_col = m_opt.GetIntValue("COLUMN");
    return "{\"status\":true}";
  }
  
  static const std::string url_base_class("/JadeMonitor/");
  if( ! url.compare(0, url_base_class.size(), url_base_class) ){
    return JadeMonitor::Post(url.substr(url_base_class.size()-1), opt);
  }
  return JadePost::Post(url, opt);
}
