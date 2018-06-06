#ifndef GUIMONITOR_HH
#define GUIMONITOR_HH

#include "JadeMonitor.hh"
#include "JadeOption.hh"
#include "qcustomplot.h"
#include <algorithm>    
#include <vector>       
#include <functional>
#include <mutex>
#include <map>
#include <iostream>

class GUIMonitor : public JadeMonitor
{
  public:
    GUIMonitor(const JadeOption& options);
    void Monitor(JadeDataFrameSP df);
    QCPColorMapData* GetADCMap();
    QCPColorMapData* GetCountMap();//add count map
    QVector<QCPGraphData> GetPedestal(int col, int row);
    QVector<QCPGraphData> GetNoise(int col, int row);
    QVector<QCPGraphData> GetHistADC(int col, int row);
    void Reset();
    void GetHistogram(const std::vector<double>& xVec, std::map<double, size_t>& hMap);
    void GetHistogram(const std::vector<int16_t>& xVec, std::map<int16_t, size_t>& hMap);

  private:
    JadeOption m_opt;
    size_t m_ev_get;
    size_t m_ev_num;
    int m_col; 
    int m_row; 
    int m_thr;//*
    std::string m_curr_time;
    JadeDataFrameSP m_df; 
    JadeDataFrameSP m_u_df; 
    std::mutex m_mx_get;
    std::mutex m_mx_set;
    uint32_t m_nx;
    uint32_t m_ny;
    
    QCPColorMapData* m_adc_map;
    QCPColorMapData* m_count_map;
    QVector<QCPGraphData> m_pedestal;
    QVector<QCPGraphData> m_noise;
    QVector<QCPGraphData> m_hist_cds_adc;

    std::vector<int16_t> m_cds_frame_adc;
    std::vector<int16_t> m_last_frame_adc;
    std::vector<int16_t> m_sum_frame_adc;
    std::vector<int16_t> m_count_frame_adc;//add count map
    std::vector<double> m_mean_frame_adc;
    std::vector<double> m_rms_frame_adc;
    std::vector<double> m_mean;
    std::vector<double> m_rms;
    std::vector<int16_t> m_cds_adc;


    struct adcFrame {
      std::vector<int16_t> cds_frame_adc;
      std::vector<double> mean_frame_adc;
      std::vector<double> rms_frame_adc;
      std::map<double, size_t> hist_mean;
      std::map<double, size_t> hist_rms;
      std::map<int16_t, size_t> hist_cds_adc;
    };
    std::shared_ptr<adcFrame> m_adcFrame; 
    std::shared_ptr<adcFrame> m_u_adcFrame; 

};
#endif