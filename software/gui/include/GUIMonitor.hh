#ifndef GUIMONITOR_HH
#define GUIMONITOR_HH

#include "JadeMonitor.hh"
#include "JadeOption.hh"
#include <queue>
#include <mutex>
#include <condition_variable>

class GUIMonitor : public JadeMonitor
{
  public:
    GUIMonitor(const JadeOption& options);
    void Monitor(JadeDataFrameSP df);
    void GetData(JadeDataFrameSP df);
    void SendData();
  private:
    JadeOption m_opt;
    size_t m_ev_get;
    size_t m_ev_num;
    std::queue<uint16_t> m_qu_data;
    std::mutex m_mx_get;
    std::mutex m_mx_send;
    std::condition_variable m_cv_data;
};

#endif
