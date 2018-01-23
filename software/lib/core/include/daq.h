#ifndef JADEPIX_INCLUDED_DAQ_H
#define JADEPIX_INCLUDED_DAQ_H

#include <cstring>
#include <string>
#include <thread>
#include <memory>
#include <atomic>

class DataTakingThread;

enum RunState {
  STATE_STOPPED,
  STATE_RUNNING,
  STATE_STOPPING,
};

enum LogLevel {
  LOG_DEBUG = 1,
  LOG_INFO = 2,
  LOG_WARNING = 3,
  LOG_ERROR = 4,
};

class EventListener {
 public:  
  virtual void logMessage(LogLevel level, const char *string) = 0;
  virtual ~EventListener() {};
};

class DataTaker {
  friend class DataTakingThread;
 public:
  DataTaker(EventListener* listener);
  ~DataTaker();
  void start_run();
  void stop_run();
  void stop_run_and_join();
  RunState get_state();
  int get_event_number();
  int get_run_number();
  size_t get_bytes_read();  
  void getRecentEvent(char *data);
  const std::string& get_filename();
  void set_filename( const std::string &fname );

 private:
  void reportThreadStopped();

 private:
  EventListener* m_listener;
  std::atomic<RunState> m_state;
  std::unique_ptr<DataTakingThread> m_threadObj;
  std::string m_filename;
};

#endif
