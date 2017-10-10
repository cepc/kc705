#include "daq2.h"
#include "daq2_private.h"
// #include <io.h>
// #include <fcntl.h>// for _O_BINARY etc.
#include <cstdio>

#include <bitset>

#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>
#include <thread>
#include <chrono>
#include <stdio.h>//added by Lu
#include <stdlib.h>//added by Lu
#include <time.h>//added by Lu
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
        m_listener->logMessage(code, os.str().c_str()); \
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

DataTaker::~DataTaker() {};

void DataTaker::start_run() {
    //std::cout << "m_listener: " << m_listener << std::endl;
    //m_listener->logMessage(LOG_INFO, "Starting run");
    DAQ_INFO("start_run");

    m_threadObj = std::make_unique<DataTakingThread>(this);
    m_threadObj->start();
    m_state = STATE_RUNNING;
    
}

void DataTaker::stop_run() {
    // std::cout << "m_listener: " << m_listener << std::endl;
    // m_listener->logMessage(LOG_INFO, "Stopping run");
    DAQ_INFO("Stopping run");

	// Don't join... if the thread is hanging (waiting for data),
	// this will hang the GUI too
    // m_threadObj->stopAndJoin();
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
    //return 456; original
	if (m_threadObj)
		return m_threadObj->m_runNumber;
	else
		return 0;//modified by Lu
}

size_t DataTaker::get_bytes_read() {
	if (m_threadObj) {
		return m_threadObj->m_bytesRead;
	}
	return 0;
}

void DataTaker::getRecentEvent(char * data) {
//	strcpy(data, "Hello Event!");*/
	if (m_threadObj) {
		std::lock_guard<std::mutex> guard(m_threadObj->eventDataMutex);
		memcpy(data, m_threadObj->m_recentEvent, 96);
	} else {
		memset(data, 0xAA, 96);
	}

}

// This is called from DataTakingThread, on the thread.
// Only use atomic access, or a lock in here.
void DataTaker::reportThreadStopped() {
	DAQ_INFO("Thread reported that it stopped.");
	m_state = STATE_STOPPED;
}

////////////////

void DataTakingThread::threadMain() {
    DAQ_INFO("Hello from threadMain!");

	FILE *fd = fopen("//./xillybus_read_32", "rb");
	//int fd = _open("//./xillybus_read_32", O_RDONLY | _O_BINARY);

	if (!fd) {
		DAQ_ERROR("Could not open stream");
		m_dataTaker->reportThreadStopped();
		return;
	}

      // Send Reset Command to FPGA
      char *reset_init = "..\\xillybus\\precompiled-demoapps\\memwrite.exe \\\\.\\xillybus_mem_8 1 15" ;

      system(reset_init);     
			
      // Sleep until FIFO in FPGA is filled 
      std::cout << "Waiting . . . " << std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(1));

	FILE *outf {nullptr};

    //const size_t frame_size = 98;
    const size_t frame_size = 96+8;
    const size_t buffer_size = 32768*4;  // FIFO depth 32768 * 4 bytes
    //const size_t read_size = 500;
    const size_t read_size = 10000;


    char buffer[buffer_size] = {0};
    char *pos = &buffer[0];
    char *used = &buffer[0];
    char *endpos = &buffer[0]+buffer_size;
    size_t bytes_avail = pos-used;

	m_runNumber += 1;
	std::ostringstream ssFilename;
	ssFilename << "run" << m_runNumber << ".bin";
	outf = fopen(ssFilename.str().c_str(), "wb");
	
	//add datetime to the beginning of the file by Lu
	time_t t=time(0);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "**********%Y%m%d%H%M%S**********", localtime(&t));
	fwrite(tmp,33,1,outf);
	fwrite("\r\n",2,1,outf);
    while (!m_stop) {
		while (bytes_avail < frame_size) {
			if (ferror(fd)) {
                DAQ_ERROR("ERROR");
				goto end;
			}

            if (pos + read_size > endpos) {
                // would write past end of buffer
                // copy what we have to beginning

                //memcpy(&buffer[0], used, pos-used); // Comment out
                //used = &buffer[0];                  // Comment out
                //pos = used + bytes_avail;           // Comment out

                std::cout << "End of Buffer Size" << std::endl;
             
                // Send Reset Command to FPGA
                char *reset = "..\\xillybus\\precompiled-demoapps\\memwrite.exe \\\\.\\xillybus_mem_8 1 15" ;

                system(reset);     
			
                // Initialization of pointers
                pos = &buffer[0];
                used = &buffer[0];
                bytes_avail = pos - used;

                // Sleep until FIFO in FPGA is filled 
                std::cout << "Waiting . . . " << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
	
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
			m_bytesRead += bytes_got;

                  std::cout << "readout size (byte) = " << bytes_got << "  " ;
                  std::cout << "total readout size (byte) = " << m_bytesRead << std::endl;

            CHECK(bytes_got > 0);

			pos += bytes_got;
            bytes_avail = pos-used;
            
            if (m_stop) goto end;
        }

		while (bytes_avail >= frame_size) {

			if (used[0] != '\xAA' || used[100] != '\xF0') {
			//if (0){
				// Not synchronized
				//DAQ_DEBUG("Unsynchronized after " << m_eventNumber << " events");

				for (char *offset = used; offset < pos; offset++) {
					if (offset[0] == '\xAA' && offset[100] == '\xF0') {
						DAQ_DEBUG("Resynchronized! Skipped " << (offset - used) << " bytes");
						// found head/tail
						used = offset;
						bytes_avail = pos - used;

						//DAQ_DEBUG("Beginning of frame: "
						//	<< std::hex << std::setfill('0')
						//	<< std::setw(2) << (int)(uint8_t)used[0] << "-"
						//	<< std::setw(2) << (int)(uint8_t)used[1] << "-"
						//	<< std::setw(2) << (int)(uint8_t)used[2] << "-"
						//	<< std::setw(2) << (int)(uint8_t)used[3] << "-"
						//	<< std::setw(2) << (int)(uint8_t)used[4] << "-"
						//	<< std::setw(2) << (int)(uint8_t)used[5] << "-"
						//	<< std::setw(2) << (int)(uint8_t)used[6] << "-"
						//);

						//DAQ_DEBUG("End of frame:       "
						//	<< std::hex << std::setfill('0')
						//	<< std::setw(2) << (int)(uint8_t)used[frame_size-7] << "-"
						//	<< std::setw(2) << (int)(uint8_t)used[frame_size-6] << "-"
						//	<< std::setw(2) << (int)(uint8_t)used[frame_size-5] << "-"
						//	<< std::setw(2) << (int)(uint8_t)used[frame_size-4] << "-"
						//	<< std::setw(2) << (int)(uint8_t)used[frame_size-3] << "-"
						//	<< std::setw(2) << (int)(uint8_t)used[frame_size-2] << "-"
						//	<< std::setw(2) << (int)(uint8_t)used[frame_size-1] << "-"
						//);


						break;
					}
				}
				
				if (bytes_avail < frame_size) {
					// get more data
					break;
				}
			}

			{
				std::lock_guard<std::mutex> guard(eventDataMutex);

				m_eventNumber++;
                        if (m_eventNumber % 10 == 0) {
                            std::cout << "Event Number = " << m_eventNumber << std::endl;
                        }

				//if (used[0] != '\xF0') {
				//	DAQ_ERROR("Wrong header");
				//	break;
				//}
				//if (used[97] != '\xAA') {
				//	DAQ_ERROR("Wrong tail");
				//	break;
				//}
				if (m_eventNumber % 2000 == 0) {
					DAQ_DEBUG("Read " << m_eventNumber << " events");
					// memcpy(m_recentEvent, used, 98);
					memcpy(m_recentEvent, used+4, 96);
					//char c = m_eventNumber / 100 % 256;
					//for (size_t i = 0; i < 98; i++) {
					//	m_recentEvent[i] = c;
					//	//if (i % 10 == 0)
					//		std::this_thread::sleep_for(1ms);
					//}
				}
			}

			// write event
			fwrite(used, 104, 1, outf);

			 //if (m_eventNumber > 10000) {
			 //	DAQ_INFO("Event limit reached.");
			 //	DAQ_INFO("Read " << m_eventNumber << " events");
			 //	goto end;
			 //}

			used += frame_size;
			bytes_avail = pos - used;



			//DAQ_DEBUG("Beginning of frame: "
			//	<< std::hex << std::setfill('0')
			//	<< std::setw(2) << (int)(uint8_t)used[0] << "-"
			//	<< std::setw(2) << (int)(uint8_t)used[1] << "-"
			//	<< std::setw(2) << (int)(uint8_t)used[2] << "-"
			//	<< std::setw(2) << (int)(uint8_t)used[3] << "-"
			//	<< std::setw(2) << (int)(uint8_t)used[4] << "-"
			//	<< std::setw(2) << (int)(uint8_t)used[5] << "-"
			//	<< std::setw(2) << (int)(uint8_t)used[6] << "-"
			//);

			//DAQ_DEBUG("End of frame:       "
			//	<< std::hex << std::setfill('0')
			//	<< std::setw(2) << (int)(uint8_t)used[frame_size - 7] << "-"
			//	<< std::setw(2) << (int)(uint8_t)used[frame_size - 6] << "-"
			//	<< std::setw(2) << (int)(uint8_t)used[frame_size - 5] << "-"
			//	<< std::setw(2) << (int)(uint8_t)used[frame_size - 4] << "-"
			//	<< std::setw(2) << (int)(uint8_t)used[frame_size - 3] << "-"
			//	<< std::setw(2) << (int)(uint8_t)used[frame_size - 2] << "-"
			//	<< std::setw(2) << (int)(uint8_t)used[frame_size - 1] << "-"
			//);

		}

    }
    end:
    DAQ_INFO("Quit taking data");
	m_dataTaker->reportThreadStopped();
	fclose(fd);
	if (outf)
		fclose(outf);
}