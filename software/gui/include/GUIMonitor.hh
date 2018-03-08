#ifndef GUIMONITOR_HH
#define GUIMONITOR_HH

#include "JadeMonitor.hh"
#include "JadeOption.hh"
#include "qcustomplot.h"
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>

class GUIMonitor : public JadeMonitor
{
  public:
    GUIMonitor(const JadeOption& options);
    void Monitor(JadeDataFrameSP df);
    void SetData(JadeDataFrameSP df);
    QCPColorMapData* GetData();
  private:
    JadeOption m_opt;
    size_t m_ev_get;
    size_t m_ev_num;
    std::vector<uint16_t> m_data;
    std::queue<std::vector<uint16_t>> m_qu_data;
    std::mutex m_mx_get;
    std::mutex m_mx_set;
    std::condition_variable m_cv_data;
    uint32_t m_nx;
    uint32_t m_ny;
    uint32_t m_offset_x;
    uint32_t m_offset_y;
    QCPColorMapData* data;
};

#endif
