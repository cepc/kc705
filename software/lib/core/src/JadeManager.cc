#include "JadeManager.hh"

using namespace std::chrono_literals;

JadeManager::JadeManager(const JadeOption &opt)
  : m_is_running(false), m_opt(opt){
}

JadeManager::~JadeManager(){
  Reset();
}

void JadeManager::SetRegCtrl(JadeRegCtrlSP ctrl){
  m_ctrl = ctrl;
}

void JadeManager::SetReader(JadeReadSP rd){
  m_rd = rd;
}

void JadeManager::SetWriter(JadeWriteSP wrt){
  m_wrt = wrt;
}

void JadeManager::SetFilter(JadeFilterSP flt){
  m_flt = flt;
}

void JadeManager::SetMonitor(JadeMonitorSP mnt){
  m_mnt = mnt;
}

uint64_t JadeManager::AsyncReading(){
  uint64_t n_df = 0;
  while (m_is_running){
    size_t nframe_per_read = 10;
    auto v_df = m_rd->Read(nframe_per_read, 10ms);
    size_t nframe_per_read_r = v_df.size();

    std::unique_lock<std::mutex> lk_out(m_mx_ev_to_dcd);
    for(auto &&df: v_df){
      m_qu_ev_to_dcd.push(df);
      n_df ++;
    }
    lk_out.unlock();
    m_cv_valid_ev_to_dcd.notify_all();
    if(nframe_per_read_r < nframe_per_read)
      break;
  }
  return n_df;
}

uint64_t JadeManager::AsyncDecoding(){
  uint64_t n_df = 0;
  while(m_is_running){
    std::unique_lock<std::mutex> lk_in(m_mx_ev_to_dcd);
    while(m_qu_ev_to_dcd.empty()){
      while(m_cv_valid_ev_to_dcd.wait_for(lk_in, 10ms) ==
          std::cv_status::timeout){
        if(!m_is_running)
          return n_df;
      }
    }
    auto df = m_qu_ev_to_dcd.front();
    m_qu_ev_to_dcd.pop();
    lk_in.unlock();
    df->Decode();
    std::unique_lock<std::mutex> lk_out(m_mx_ev_to_flt);
    m_qu_ev_to_flt.push(df);
    n_df ++;
    lk_out.unlock();
    m_cv_valid_ev_to_flt.notify_all();   
  }
  return n_df;
}

uint64_t JadeManager::AsyncFiltering(){
  uint64_t n_df = 0;
  while(m_is_running){
    std::unique_lock<std::mutex> lk_in(m_mx_ev_to_flt);
    while(m_qu_ev_to_flt.empty()){
      while(m_cv_valid_ev_to_flt.wait_for(lk_in, 10ms) ==
          std::cv_status::timeout){
        if(!m_is_running){
          return n_df;
        }
      }
    }
    auto df = m_qu_ev_to_flt.front();
    m_qu_ev_to_flt.pop();
    lk_in.unlock();
    JadeDataFrameSP df_r = df;
    if(m_flt)
      df_r = m_flt->Filter(df);
    if(df_r){
      std::unique_lock<std::mutex> lk_out_wrt(m_mx_ev_to_wrt);
      m_qu_ev_to_wrt.push(df_r);
      lk_out_wrt.unlock();
      m_cv_valid_ev_to_wrt.notify_all();
      
      std::unique_lock<std::mutex> lk_out_mnt(m_mx_ev_to_mnt);
      m_qu_ev_to_mnt.push(df_r);
      lk_out_mnt.unlock();
      m_cv_valid_ev_to_mnt.notify_all();
      n_df ++;
    }
  }
  return n_df;
}

uint64_t JadeManager::AsyncWriting(){
  uint64_t n_df = 0;
  while(m_is_running){
    std::unique_lock<std::mutex> lk_in(m_mx_ev_to_wrt);
    while(m_qu_ev_to_wrt.empty()){
      while(m_cv_valid_ev_to_wrt.wait_for(lk_in, 10ms)
          ==std::cv_status::timeout){
        if(!m_is_running){
          return n_df;
        }
      }
    }
    auto df = m_qu_ev_to_wrt.front();
    m_qu_ev_to_wrt.pop();
    lk_in.unlock();
    if(m_wrt)
      m_wrt->Write(df);
    n_df ++;
  }
  return n_df;
}

uint64_t JadeManager::AsyncMonitoring(){
  uint64_t n_df = 0;
  while(m_is_running){
    std::unique_lock<std::mutex> lk_in(m_mx_ev_to_mnt);
    while(m_qu_ev_to_mnt.empty()){
      while(m_cv_valid_ev_to_mnt.wait_for(lk_in, 10ms)
          ==std::cv_status::timeout){
        if(!m_is_running){
          return n_df;
        }
      }
    }
    if(m_qu_ev_to_mnt.size()>100000){
      std::cerr<<"JadeManager:: the data queue to monitor is full\n";
      std::cerr<<"JadeManager:: the monitor is too slow to monitor data\n";
      throw;
    }
    auto df = m_qu_ev_to_mnt.front();
    m_qu_ev_to_mnt.pop();
    lk_in.unlock();
    if(m_mnt)
      m_mnt->Monitor(df);
    n_df ++;
  }
  return n_df;
}

void JadeManager::StartDataTaking(){
  if(!m_rd || !m_flt || !m_wrt || !m_mnt || !m_ctrl) {
    std::cerr<<"JadeManager: m_rd, m_flt, m_wrt, m_mnt or m_ctrl is not set"<<std::endl;
    throw;
  }

  m_wrt->Open();
  m_ctrl->SendCommand("START"); 
  // while(DeviceStatus("RUN_TYPE") != "STARTED"){
  //   ;
  // }
  m_is_running = true;
  m_fut_async_rd = std::async(std::launch::async,
      &JadeManager::AsyncReading, this);

  m_fut_async_dcd = std::async(std::launch::async,
      &JadeManager::AsyncDecoding, this);

  m_fut_async_flt = std::async(std::launch::async,
      &JadeManager::AsyncFiltering, this);

  m_fut_async_wrt = std::async(std::launch::async,
      &JadeManager::AsyncWriting, this);

  m_fut_async_mnt = std::async(std::launch::async,
      &JadeManager::AsyncMonitoring, this);  
}

void JadeManager::StopDataTaking(){
  DeviceControl("STOP");
  // while(DeviceStatus("RUN_TYPE") != "STOPPED"){
  //   ;
  // }
  std::this_thread::sleep_for(std::chrono::milliseconds(500));//TOBE REMOVEd
  
  m_is_running = false;
  if(m_fut_async_rd.valid())
    m_fut_async_rd.get();
  if(m_fut_async_dcd.valid())
    m_fut_async_dcd.get();
  if(m_fut_async_flt.valid())
    m_fut_async_flt.get();
  if(m_fut_async_wrt.valid())
    m_fut_async_wrt.get();
  if(m_fut_async_mnt.valid())
    m_fut_async_mnt.get();
  m_qu_ev_to_dcd=decltype(m_qu_ev_to_dcd)();
  m_qu_ev_to_flt=decltype(m_qu_ev_to_flt)();
  m_qu_ev_to_wrt=decltype(m_qu_ev_to_wrt)();
  m_qu_ev_to_mnt=decltype(m_qu_ev_to_mnt)();

  m_wrt->Close();
}

void JadeManager::Reset(){
  m_is_running = false;
  if(m_fut_async_rd.valid())
    m_fut_async_rd.get();
  if(m_fut_async_dcd.valid())
    m_fut_async_dcd.get();
  if(m_fut_async_flt.valid())
    m_fut_async_flt.get();
  if(m_fut_async_wrt.valid())
    m_fut_async_wrt.get();
  if(m_fut_async_mnt.valid())
    m_fut_async_mnt.get();
  m_qu_ev_to_dcd=decltype(m_qu_ev_to_dcd)();
  m_qu_ev_to_flt=decltype(m_qu_ev_to_flt)();
  m_qu_ev_to_wrt=decltype(m_qu_ev_to_wrt)();
  m_qu_ev_to_mnt=decltype(m_qu_ev_to_mnt)();
  if(m_ctrl)
    m_ctrl->Reset();
  if(m_rd)
    m_rd->Reset();
  if(m_flt)
    m_flt->Reset();
  if(m_wrt)
    m_wrt->Reset();
  if(m_mnt)
    m_mnt->Reset();
}

void JadeManager::DeviceConnect(){
  m_rd->Open();
  m_ctrl->Open();
}

void JadeManager::DeviceDisconnect(){
  m_rd->Close();
  m_ctrl->Close();
}

void JadeManager::DeviceControl(const std::string &cmd){
  m_ctrl->SendCommand(cmd);
}

const std::string& JadeManager::DeviceStatus(const std::string &type){
  return m_ctrl->GetStatus(type);
}
