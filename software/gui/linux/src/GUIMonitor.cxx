#include "GUIMonitor.hh"

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
    , m_thr(0) //*
{
  m_ev_get = m_opt.GetIntValue("PRINT_EVENT_N");
  m_col = m_opt.GetIntValue("COLUMN");
  m_row = m_opt.GetIntValue("ROW");
  m_thr = m_opt.GetIntValue("ADC_THREASHOLD");

  m_adc_map = std::shared_ptr<TH2F>(new TH2F("ADC_map", "ADC map", m_nx, 0, m_nx, m_ny, 0, m_ny));
  m_adc_hist = std::shared_ptr<TH1F>(new TH1F("ADC", "ADC", 2000, -10000, 10000));
}

void GUIMonitor::Monitor(JadeDataFrameSP df)
{
  if (m_ev_get != 0 && m_ev_num % m_ev_get < 10) {
    df->Print(std::cout);
    std::unique_lock<std::mutex> lk_in(m_mx_set);
    m_df = df;
    lk_in.unlock();

    if ((m_ev_num) == 0) {
      m_ev_num++;
      return;
    }

    auto cds_adc = m_df->GetFrameCDS();
    for (size_t iy = 0; iy < m_ny; iy++)
      for (size_t ix = 0; ix < m_nx; ix++) {
        auto pos = ix + m_nx * iy;
        auto value = cds_adc.at(pos);
        if (std::abs(value) > m_thr) {
          m_adc_map->Fill(m_nx-ix, m_ny-iy);
        }
        if (ix == m_col && iy == m_row) {
          m_adc_hist->Fill(value);
        }
      }

    std::unique_lock<std::mutex> lk_out(m_mx_get);
    m_adc_map_clone = m_adc_map;
    m_adc_hist_clone = m_adc_hist;
    lk_out.unlock();
  }
  m_ev_num++;
}

void GUIMonitor::Reset()
{
}

std::shared_ptr<TH2F> GUIMonitor::GetADCMap()
{
  return m_adc_map_clone;
}

std::shared_ptr<TH1F> GUIMonitor::GetADCHist()
{
  return m_adc_hist_clone;
}
