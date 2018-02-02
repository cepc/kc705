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
  std::cout<<"JadeManager: deleted"<<std::endl;
}

uint64_t JadeManager::AsyncReading(){
  uint64_t n_df = 0;
  // uint debug_n = 2;
  while (m_is_running){
    // if(debug_n ==0)
    //   break;
    // debug_n --;
    size_t nframe_per_read = 10;
    auto v_df = m_rd->Read(nframe_per_read, 10ms);
    size_t nframe_per_read_r = v_df.size();
      
    std::unique_lock<std::mutex> lk_out(m_mx_ev_to_dcd);
    for(auto &&df: v_df){
      m_qu_ev_to_dcd.push(std::move(df));
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
    std::cout<<"decode"<<std::endl;
    std::unique_lock<std::mutex> lk_out(m_mx_ev_to_dcd);
    m_qu_ev_to_flt.push(std::move(df));
    n_df ++;
    lk_out.unlock();
    m_cv_valid_ev_to_flt.notify_all();   
  }
  return n_df;
}

uint64_t JadeManager::AsyncFiltering(){
  std::cout<< ">>>>>>>>>>>>>>>>>>>>>>>>>>Filtering thread"<<std::endl;
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
    std::cout<<"filtering"<<std::endl;
    auto df_r = m_flt->Filter(std::move(df));
    if(df_r){
      std::cout<<"pass"<<std::endl;
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
  std::cout<< "writing thread"<<std::endl;
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
    if(!df)
      std::cerr<<"async wrt, empty ptr"<<std::endl;
    m_qu_ev_to_wrt.pop();
    lk_in.unlock();
    m_wrt->Write(std::move(df));
    n_df ++;
  }
  return n_df;
}

void JadeManager::Start(const std::string &file_in,
			const std::string &file_out){
  m_is_running = true;

  m_rd.reset(new JadeRead(file_in, ""));
  m_flt.reset(new JadeFilter(""));
  m_wrt.reset(new JadeWrite(file_out, ""));

  m_fut_async_rd = std::async(std::launch::async,
  			      &JadeManager::AsyncReading, this);

  m_fut_async_dcd = std::async(std::launch::async,
  			      &JadeManager::AsyncDecoding, this);
  
  m_fut_async_flt = std::async(std::launch::async,
  			       &JadeManager::AsyncFiltering, this);
  
  m_fut_async_wrt = std::async(std::launch::async,
  			       &JadeManager::AsyncWriting, this);
}
