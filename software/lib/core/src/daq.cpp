#include "daq.h"
#include "daq_private.h"

#include <cstdio>
#include <bitset>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>
using namespace std::chrono_literals;

#define DAQ_LOG(code, x)				\
  do {							\
    std::ostringstream os;				\
    os << x;						\
    m_listener->logMessage(code, os.str().c_str());	\
  } while (false)

#define DAQ_DEBUG(x)    DAQ_LOG(LOG_DEBUG, x)
#define DAQ_INFO(x)     DAQ_LOG(LOG_INFO, x)
#define DAQ_WARNING(x)  DAQ_LOG(LOG_WARNING, x)
#define DAQ_ERROR(x)    DAQ_LOG(LOG_ERROR, x)


DataTaker::DataTaker(EventListener *listener)
  :m_listener(listener), m_state(STATE_STOPPED),
   m_threadObj(nullptr), m_max_event_number(10000){  
  DAQ_INFO("In constructor");
}

DataTaker::~DataTaker() {};

void DataTaker::start_run() {
  DAQ_INFO("start_run");

  m_threadObj = std::make_unique<DataTakingThread>(this);
  m_threadObj->start();
  m_state = STATE_RUNNING;
    
}

void DataTaker::stop_run() {
  DAQ_INFO("Stopping run");
  m_state = STATE_STOPPING;
  m_threadObj->stop();
  // TODO: let the thread communicate state back
}

void DataTaker::stop_run_and_join() {
  stop_run();
  m_threadObj->stopAndJoin();
}

RunState DataTaker::get_state() {
  return m_state;
}

int DataTaker::get_event_number() {
  if (m_threadObj)
    return m_threadObj->m_eventNumber;
  else
    return 0;
}

int DataTaker::get_run_number() {
  if (m_threadObj)
    return m_threadObj->m_runNumber;
  else
    return 0;
}

size_t DataTaker::get_bytes_read() {
  if (m_threadObj) {
    return m_threadObj->m_bytesRead;
  }
  return 0;
}

void DataTaker::get_recent_event(char * data) {
  if (m_threadObj) {
    std::lock_guard<std::mutex> guard(m_threadObj->eventDataMutex);
    memcpy(data, m_threadObj->m_recentEvent, 1928);
  } else {
    memset(data, 0xAA, 1928);
  }

}

void DataTaker::re_set()
{
	// Send Reset Command to FPGA
  std::string reset_init("..\\xillybus\\precompiled-demoapps\\memwrite.exe \\\\.\\xillybus_mem_8 1 15");
  system(reset_init.c_str());
}

// This is called from DataTakingThread, on the thread.
// Only use atomic access, or a lock in here.
void DataTaker::reportThreadStopped() {
  DAQ_INFO("Thread reported that it stopped.");
  m_state = STATE_STOPPED;
}


const std::string& DataTaker::get_filename(){
  return m_filename;
}

void DataTaker::set_filename( const std::string &fname ){
  m_filename = fname;
}


////////////////

