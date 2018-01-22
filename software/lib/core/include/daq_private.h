#ifndef JADEPIX_INCLUDED_DAQ_PRIVATE_H
#define JADEPIX_INCLUDED_DAQ_PRIVATE_H

#include <atomic>
#include <thread>
#include <mutex>

class EventListener;

class DataTakingThread {
 public:
  DataTakingThread(DataTaker *dataTaker);
  ~DataTakingThread();
  void start();
  void stop();
  void stopAndJoin();

 private:  
  void threadMain();

 public:
  std::atomic<int> m_eventNumber;
  std::atomic<int> m_runNumber;
  std::atomic<size_t> m_bytesRead;
  char m_recentEvent[1928];
  std::mutex eventDataMutex;
  
 private:
  DataTaker *m_dataTaker;
  EventListener *m_listener;
  std::thread m_thread;
  std::atomic<bool> m_stop;
};

#endif
