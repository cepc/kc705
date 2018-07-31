#ifndef GUIMONITOR_HH
#define GUIMONITOR_HH

#include "JadeCore.hh"
#include <TH1.h>
#include <TH2.h>
#include <TRandom.h>
#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <mutex>
#include <vector>

class GUIMonitor : public JadeMonitor {
public:
  GUIMonitor(const JadeOption& options);
  ~GUIMonitor() override{};
  void Monitor(JadeDataFrameSP df) override;
  void Reset() override;
  JadeOption Post(const std::string &url, const JadeOption &opt) override;
  
  std::shared_ptr<TH2F> GetADCMap();
  std::shared_ptr<TH2F> GetADCCounts();
  std::vector<std::shared_ptr<TH1F>> GetADCHist();

private:
  JadeOption m_opt;
  size_t m_ev_get;
  size_t m_ev_num;
  int m_col;
  int m_row;
  int m_nbins;
  int m_thr; //*
  JadeDataFrameSP m_df;
  JadeDataFrameSP m_last_df;
  std::mutex m_mx_get;
  uint32_t m_nx;
  uint32_t m_ny;
  bool m_enbale_print_events;
  float m_monitor_percent;
  std::shared_ptr<TH2F> m_adc_counts;
  std::shared_ptr<TH2F> m_adc_counts_clone;
  std::shared_ptr<TH2F> m_adc_map;
  std::shared_ptr<TH2F> m_adc_map_clone;
  std::vector<std::shared_ptr<TH1F>> m_adc_hist;
  std::vector<std::shared_ptr<TH1F>> m_adc_hist_clone;
};

#endif
