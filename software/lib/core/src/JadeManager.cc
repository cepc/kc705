#include "JadeManager.hh"


JadeManager::JadeManager(const std::string &dev_path)
  :m_dev_path(dev_path), m_is_running(false){
}

JadeManager::~JadeManager(){
  m_is_running = false;
  if(m_fut_async_wrt.valid()){
    m_fut_async_wrt.get();
  }
}

uint64_t JadeManager::AsyncReading(){
  m_fut_async_dcd = std::async(std::launch::async,
			       &JadeManager::AsyncDecoding, this); 
  while (m_is_running){
    //TODO: read
    // std::this_thread::sleep_for(std::chrono::milliseconds(100));
    //TODO: add data to queue
    m_cv_valid_ev_to_dcd.notify_all();
  }
  return 0;
}

uint64_t JadeManager::AsyncDecoding(){
  m_fut_async_flt = std::async(std::launch::async,
			       &JadeManager::AsyncFiltering, this); 
  while(m_is_running){
    std::unique_lock<std::mutex> lk(m_mx_ev_to_dcd);
    if(m_qu_ev_to_dcd.empty()){
      while(m_cv_valid_ev_to_dcd.wait_for(lk, std::chrono::seconds(1))
	    ==std::cv_status::timeout){
	if(!m_fut_async_rd.valid()){
	  return 0;
	}
      }
    }
    auto ev = std::move(m_qu_ev_to_dcd.front());
    m_qu_ev_to_dcd.pop();
    lk.unlock();
    //TODO: decoding
    //to queue
    m_cv_valid_ev_to_flt.notify_all();    
  }
  return 0;
}

uint64_t JadeManager::AsyncFiltering(){
  m_fut_async_wrt = std::async(std::launch::async,
			       &JadeManager::AsyncWriting, this); 
  while(m_is_running){
    std::unique_lock<std::mutex> lk(m_mx_ev_to_flt);
    if(m_qu_ev_to_flt.empty()){
      while(m_cv_valid_ev_to_flt.wait_for(lk, std::chrono::seconds(1))
	    ==std::cv_status::timeout){
	if(!m_is_running){
	  return 0;
	}
      }
    }
    auto ev = std::move(m_qu_ev_to_flt.front());
    m_qu_ev_to_flt.pop();
    lk.unlock();
    //TODO: filtering
    //to queue
    m_cv_valid_ev_to_wrt.notify_all();    
  }
  return 0;
}

uint64_t JadeManager::AsyncWriting(){
  while(m_is_running){
    std::unique_lock<std::mutex> lk(m_mx_ev_to_wrt);
    if(m_qu_ev_to_wrt.empty()){
      while(m_cv_valid_ev_to_wrt.wait_for(lk, std::chrono::seconds(1))
	    ==std::cv_status::timeout){
	if(!m_is_running){
	  return 0;
	}
      }
    }
    auto ev = std::move(m_qu_ev_to_wrt.front());
    m_qu_ev_to_wrt.pop();
    lk.unlock();
    //TODO: write
  }
  return 0;
}

void JadeManager::Start(const std::string &addr){
  //TODO: check if it is stop
  m_fut_async_rd = std::async(std::launch::async, &JadeManager::AsyncReading, this); 
}
  
