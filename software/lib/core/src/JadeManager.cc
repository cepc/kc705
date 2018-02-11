#include "JadeManager.hh"

using namespace std::chrono_literals;

JadeManager::JadeManager()
  : m_is_running(false){
  }

JadeManager::~JadeManager(){
  m_is_running = false;
  if(m_fut_async_rd.valid())
    m_fut_async_rd.get();
  if(m_fut_async_dcd.valid())
    m_fut_async_dcd.get();
  if(m_fut_async_flt.valid())
    m_fut_async_flt.get();
  if(m_fut_async_wrt.valid())
    m_fut_async_wrt.get();
}

void JadeManager::SetReader(std::unique_ptr<JadeRead> &&rd){
  m_rd.reset();
  m_rd = std::move(rd);
}

void JadeManager::SetWriter(std::unique_ptr<JadeWrite> &&wrt){
  m_wrt.reset();
  m_wrt = std::move(wrt);
}

void JadeManager::SetFilter(std::unique_ptr<JadeFilter> &&flt){
  m_flt.reset();
  m_flt = std::move(flt);
}

//uint64_t JadeManager::AsyncReading(){
//  uint64_t n_df = 0;
//  while (m_is_running){
//    size_t nframe_per_read = 10;
//    auto v_df = m_rd->Read(nframe_per_read, 10ms);
//    size_t nframe_per_read_r = v_df.size();
//
//    std::unique_lock<std::mutex> lk_out(m_mx_ev_to_dcd);
//    for(auto &&df: v_df){
//      m_qu_ev_to_dcd.push(std::move(df));
//      n_df ++;
//    }
//    lk_out.unlock();
//    m_cv_valid_ev_to_dcd.notify_all();
//    if(nframe_per_read_r < nframe_per_read)
//      break;
//  }
//  return n_df;
//}

uint64_t JadeManager::AsyncReading(){
  uint64_t n_df = 0;
  while (m_is_running){
    size_t nframe_per_read = 10;
    auto v_df = m_rd->Read(nframe_per_read, 10ms);
    size_t nframe_per_read_r = v_df.size();

    std::unique_lock<std::mutex> lk_out(m_mx_ev_to_wrt);
    for(auto &&df: v_df){
      m_qu_ev_to_wrt.push(std::move(df));
      n_df ++;
    }
    lk_out.unlock();
    m_cv_valid_ev_to_wrt.notify_all();
    if(nframe_per_read_r < nframe_per_read)
      break;
  }
  return n_df;
}

uint64_t JadeManager::AsyncDecoding(){
  uint64_t n_df = 0;
  while(m_is_running){
    std::unique_lock<std::mutex> lk_in(m_mx_ev_to_dcd);
    if(m_qu_ev_to_dcd.empty()){
      while(m_cv_valid_ev_to_dcd.wait_for(lk_in, 10ms) ==
          std::cv_status::timeout){
        if(!m_is_running)
          return n_df;
      }
    }
    auto df = std::move(m_qu_ev_to_dcd.front());
    m_qu_ev_to_dcd.pop();
    lk_in.unlock();
    df->Decode();
    std::unique_lock<std::mutex> lk_out(m_mx_ev_to_dcd);
    m_qu_ev_to_flt.push(std::move(df));
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
    if(m_qu_ev_to_flt.empty()){
      while(m_cv_valid_ev_to_flt.wait_for(lk_in, 1s) ==
          std::cv_status::timeout){
        if(!m_is_running){
          return n_df;
        }
      }
    }
    auto df = std::move(m_qu_ev_to_flt.front());
    m_qu_ev_to_flt.pop();
    lk_in.unlock();
    auto df_r = m_flt->Filter(std::move(df));
    if(df_r){
      std::unique_lock<std::mutex> lk_out(m_mx_ev_to_wrt);
      m_qu_ev_to_wrt.push(std::move(df_r));
      n_df ++;
      lk_out.unlock();
      m_cv_valid_ev_to_wrt.notify_all();
    }
  }
  return n_df;
}

uint64_t JadeManager::AsyncWriting(){
  uint64_t n_df = 0;
  while(m_is_running){
    std::unique_lock<std::mutex> lk_in(m_mx_ev_to_wrt);
    if(m_qu_ev_to_wrt.empty()){
      while(m_cv_valid_ev_to_wrt.wait_for(lk_in, 1s)
          ==std::cv_status::timeout){
        if(!m_is_running){
          return n_df;
        }
      }
    }
    auto df = std::move(m_qu_ev_to_wrt.front());
    m_qu_ev_to_wrt.pop();
    lk_in.unlock();
    m_wrt->Write(std::move(df));
    n_df ++;
  }
  return n_df;
}

void JadeManager::Start(){
  //if(!m_rd || !m_flt || !m_wrt){
  //  std::cerr<<"JadeManager: m_rd or m_flt or m_wrt is not set"<<std::endl;
  //  throw;
  //}

  if(!m_rd || !m_wrt){
    std::cerr<<"JadeManager: m_rd or m_wrt is not set"<<std::endl;
    throw;
  }

  m_is_running = true;
  m_fut_async_rd = std::async(std::launch::async,
      &JadeManager::AsyncReading, this);

  //m_fut_async_dcd = std::async(std::launch::async,
  //    &JadeManager::AsyncDecoding, this);

  //m_fut_async_flt = std::async(std::launch::async,
  //    &JadeManager::AsyncFiltering, this);

  m_fut_async_wrt = std::async(std::launch::async,
      &JadeManager::AsyncWriting, this);
}

void JadeManager::Stop(){
  m_is_running = false;
  if(m_fut_async_rd.valid())
    m_fut_async_rd.get();
  if(m_fut_async_dcd.valid())
    m_fut_async_dcd.get();
  if(m_fut_async_flt.valid())
    m_fut_async_flt.get();
  if(m_fut_async_wrt.valid())
    m_fut_async_wrt.get();
  m_qu_ev_to_dcd=decltype(m_qu_ev_to_dcd)();
  //m_qu_ev_to_flt=decltype(m_qu_ev_to_flt)();
  m_qu_ev_to_wrt=decltype(m_qu_ev_to_wrt)();

  m_rd.reset();
  //m_flt.reset();
  m_wrt.reset();
}
