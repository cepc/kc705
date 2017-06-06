#include "daq2.h"

// #include <io.h>
// #include <fcntl.h>// for _O_BINARY etc.
#include <cstdio>

#include <iostream>
#include <sstream>
#include <fstream>
#include <thread>
#include <chrono>
using namespace std::chrono_literals;

// Debug macro, crashes if the condition is false
#define CHECK(ARG)                                      \
    do {                                                \
        const bool result = ARG;                        \
        if( ! result ) {                                \
            std::ostringstream os;                      \
            const char *pos = strrchr(__FILE__, '/');   \
            const char *fn = pos ? pos+1 : __FILE__;    \
            os << "Fatal error at " << fn << ":"        \
                << __LINE__ << ": \"" << #ARG << "\"";  \
            std::cerr << os.str() << std::endl;         \
            throw std::runtime_error(os.str());         \
        }                                               \
    } while (false)

// General log macro, calls m_listener->logMessage.  Use the
// specific macros (DAQ_DEBUG, DAQ_INFO, ...) defined below.
// This is modelled on the ATH_MSG_INFO macros, and you can
// use c++ stream formatting.  Example:
//
//     DAQ_INFO("Hello " << 42);
//
#define DAQ_LOG(code, x)                                     \
    do {                                                     \
        std::ostringstream os;                               \
        os << x;                                             \
        m_listener->logMessage(LOG_DEBUG, os.str().c_str()); \
    } while (false)

#define DAQ_DEBUG(x)    DAQ_LOG(LOG_DEBUG, x)
#define DAQ_INFO(x)     DAQ_LOG(LOG_INFO, x)
#define DAQ_WARNING(x)  DAQ_LOG(LOG_WARNING, x)
#define DAQ_ERROR(x)    DAQ_LOG(LOG_ERROR, x)


DataTaker::DataTaker(EventListener *listener): m_listener(listener), m_state(STATE_STOPPED), m_threadObj(nullptr) {
    //std::cout << "m_listener: " << m_listener << std::endl;
    // m_listener->logMessage(LOG_INFO, "In constructor");
    DAQ_INFO("In constructor");
}

void DataTaker::start_run() {
    //std::cout << "m_listener: " << m_listener << std::endl;
    //m_listener->logMessage(LOG_INFO, "Starting run");
    DAQ_INFO("start_run");

    m_threadObj = std::make_unique<DataTakingThread>(m_listener);
    m_threadObj->start();
    m_state = STATE_RUNNING;
    
}

void DataTaker::stop_run() {
    // std::cout << "m_listener: " << m_listener << std::endl;
    // m_listener->logMessage(LOG_INFO, "Stopping run");
    DAQ_INFO("Stopping run");

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
    DAQ_INFO("Hello from threadMain!");

	FILE *fd = fopen("//./xillybus_read_32", "rb");
	//int fd = _open("//./xillybus_read_32", O_RDONLY | _O_BINARY);

    const size_t frame_size = 98;
    const size_t buffer_size = 20000;
    const size_t read_size = 10000;

    int nevents = 0;
    char buffer[buffer_size] = {0};
    char *pos = &buffer[0];
    char *used = &buffer[0];
    char *endpos = &buffer[0]+buffer_size;
    size_t bytes_avail = pos-used;

    while (!m_stop) {
		while (bytes_avail < frame_size) {
			if (ferror(fd)) {
                DAQ_ERROR("ERROR");
				goto end;
			}

            if (pos + read_size > endpos) {
                // would write past end of buffer
                // copy what we have to beginning
                memcpy(&buffer[0], used, pos-used);
                used = &buffer[0];
                pos = used + bytes_avail;
            }
			if (pos + read_size > endpos) {
                // This shouldn't happen, we cannot read past the end of the buffer
				std::cout << "pos: " << (void*)pos << std::endl;
				std::cout << "pos - &buffer[0]: " << (size_t)(pos - &buffer[0]) << std::endl;
				std::cout << "read_size: " << read_size << std::endl;
				std::cout << "endpos: " << (void*)endpos << std::endl;
				std::cout << "bytes_avail: " << bytes_avail << std::endl;
			}
			
			CHECK(pos + read_size <= endpos);
			CHECK(used <= pos);
			CHECK(pos - used == bytes_avail);
			CHECK(bytes_avail <= buffer_size);

			size_t bytes_got = fread(pos, 1, read_size, fd);

            CHECK(bytes_got > 0);

			pos += bytes_got;
            bytes_avail = pos-used;
            
            if (m_stop) goto end;
        }

		while (bytes_avail >= frame_size) {
			nevents++;
			if (used[0] != '\xF0') {
				DAQ_ERROR("Wrong header");
				break;
			}
			if (used[97] != '\xAA') {
				DAQ_ERROR("Wrong tail");
				break;
			}
			if (nevents % 1000 == 0) {
				DAQ_DEBUG("Read " << nevents << " events");
			}

			used += frame_size;
			bytes_avail = pos - used;
		}

    }
    end:
    DAQ_INFO("Quit taking data");
	fclose(fd);
}
