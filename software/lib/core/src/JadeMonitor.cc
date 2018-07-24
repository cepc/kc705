#include "JadeMonitor.hh"

JadeMonitor::JadeMonitor(const JadeOption& opt)
    : m_opt(opt)
    , m_ev_n(0)
    , m_ev_print(0)
    , m_last_df({ 0 })
    , m_last_df_n(0)
    , m_enable_print_discon(false)
    , m_enable_cds(true)
{
  m_ev_print = opt.GetIntValue("PRINT_EVENT_N");
  m_enable_print_discon = opt.GetBoolValue("PRINT_EVENT_DISCONTINUOUS");
  m_enable_cds = opt.GetBoolValue("ENABLE_CDS");
}

JadeMonitor::~JadeMonitor()
{
}

void JadeMonitor::Reset()
{
  m_ev_n = 0;
}

void JadeMonitor::Monitor(JadeDataFrameSP df)
{
  if (m_enable_cds) {
    if (m_ev_n == 0) {
      m_last_df = df;
    } else {
      df->CDS(*m_last_df);
      m_last_df = df;
    }
  }

  if (m_ev_print != 0 && m_ev_n % m_ev_print == 0) {
    df->Print(std::cout);
    if (m_enable_cds) {
      df->PrintCDS(std::cout);
    }
  }

  m_ev_n++;

  if (m_enable_print_discon) {
    uint32_t df_n = df->GetFrameCount();
    if (m_last_df_n != 0 && m_last_df_n + 1 != df_n) {
      std::cout << "JadeMonitor: data frame counter is discontinuous "
                << m_last_df_n << " =>  " << df_n << std::endl;
    }
    m_last_df_n = df_n;
  }
}
