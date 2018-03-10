#ifndef GUIMONITOR_HH
#define GUIMONITOR_HH

#include "JadeMonitor.hh"
#include "JadeOption.hh"
#include "qcustomplot.h"
#include <mutex>

class GUIMonitor : public JadeMonitor
{
  public:
    GUIMonitor(const JadeOption& options);
    void Monitor(JadeDataFrameSP df);
    void ProcessData();
    QCPColorMapData* GetADCMap();
    QCPGraph* GetPedestal(int col, int row);
  
  private:
    JadeOption m_opt;
    size_t m_ev_get;
    size_t m_ev_num;
    JadeDataFrameSP m_df; 
    JadeDataFrameSP m_u_df; 
    std::mutex m_mx_get;
    std::mutex m_mx_set;
    uint32_t m_nx;
    uint32_t m_ny;
    uint32_t m_offset_x;
    uint32_t m_offset_y;
    QCPColorMapData* m_adc_map;
    QCPGraph* m_pedestal;

    uint32_t m_cds_frame_adc[16][48];
    uint32_t m_last_frame_adc[16][48];   
    uint32_t m_sum_frame_adc[16][48];
    double m_mean_adc[16][48];
    double m_rms_adc[16][48];
};

#endif
