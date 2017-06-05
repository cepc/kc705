#include "daq2.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <thread>
#include <chrono>
using namespace std::chrono_literals;

DataTaker::DataTaker(EventListener *listener): m_listener(listener), m_state(STATE_STOPPED), m_threadObj(nullptr) {
    std::cout << "m_listener: " << m_listener << std::endl;
    m_listener->logMessage(LOG_INFO, "In constructor");
}

void DataTaker::start_run() {
    std::cout << "m_listener: " << m_listener << std::endl;
    m_listener->logMessage(LOG_INFO, "Starting run");

    m_threadObj = std::make_unique<DataTakingThread>(m_listener);
    m_threadObj->start();
    m_state = STATE_RUNNING;
    
}

void DataTaker::stop_run() {
    std::cout << "m_listener: " << m_listener << std::endl;
    m_listener->logMessage(LOG_INFO, "Stopping run");

    m_threadObj->stopAndJoin();
    m_state = STATE_STOPPED;
}

RunState DataTaker::get_state() {
    return m_state;
}

int DataTaker::get_event_number() {
    return 123;
}

int DataTaker::get_run_number() {
    return 456;
}

////////////////

void DataTakingThread::threadMain() {
    m_listener->logMessage(LOG_INFO, "Hello from threadMain");

    std::ifstream input("//./xillybus_read_32");
    
    int nevents = 0;
    while (!m_stop) {
        char buffer[98] = {0};
        input.read(&buffer[0], sizeof(buffer));
        if (input.gcount() != sizeof(buffer)) {
            std::ostringstream os;
            os << "Warning, only " << input.gcount() << " bytes read";
            m_listener->logMessage(LOG_WARNING, os.str().c_str());
        }
        nevents++;
        if (buffer[0] != '\xF0') {
            m_listener->logMessage(LOG_ERROR, "Wrong header");
            break;
        }
        if (buffer[97] != '\xAA') {
            m_listener->logMessage(LOG_ERROR, "Wrong tail");
            break;
        }
        //std::this_thread::sleep_for(1s);
        if (nevents % 100 == 0) {
            std::ostringstream os;
            os << "Read " << nevents << " events";
            m_listener->logMessage(LOG_DEBUG, os.str().c_str());
        }
    }
    m_listener->logMessage(LOG_INFO, "Quit taking data");
}
