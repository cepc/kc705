#include "daq.h"
#include "daq_private.h"

#include <string>
#include <sstream>
#include <iostream>

#define CHECK(ARG)                               \
  do                                             \
  {                                              \
    const bool result = ARG;                     \
    if (!result)                                 \
    {                                            \
      std::ostringstream os;                     \
      const char *pos = strrchr(__FILE__, '/');  \
      const char *fn = pos ? pos + 1 : __FILE__; \
      os << "Fatal error at " << fn << ":"       \
         << __LINE__ << ": \"" << #ARG << "\"";  \
      std::cerr << os.str() << std::endl;        \
      throw std::runtime_error(os.str());        \
    }                                            \
  } while (false)

DataTakingThread::DataTakingThread(DataTaker *dataTaker) : m_dataTaker(dataTaker), m_listener(dataTaker->m_listener),
                                                           m_thread(), m_stop(false), m_eventNumber(0), m_runNumber(0),
                                                           m_bytesRead(0)
{
  memset(m_recentEvent, 0, sizeof(m_recentEvent));
}

DataTakingThread::~DataTakingThread()
{
  stopAndJoin();
}

void DataTakingThread::start()
{
  m_thread = std::thread(&DataTakingThread::threadMain, this);
}

void DataTakingThread::stop()
{
  m_stop = true;
}

void DataTakingThread::stopAndJoin()
{
  m_stop = true;
  if (m_thread.joinable())
    m_thread.join();
}

void DataTakingThread::threadMain()
{

#ifdef _WIN32
  FILE *fd = fopen("//./xillybus_read_32", "rb");
#else
  //FILE *fd = fopen("/dev/xillybus_read_32", "rb");
  FILE *fd = fopen("/tmp/test_pipe", "rb");
#endif

  if (!fd)
  {
    std::cerr << "Could not open stream";
    m_dataTaker->reportThreadStopped();
    return;
  }
  else
  {
    //DAQ_INFO("The pipe is open! ");
    std::cout << "The pipe is open! " << std::endl;
  }

  // Send Reset Command to FPGA
  if (!m_dataTaker->get_simulate_state())
  {
    m_dataTaker->re_set();
  }

  // Sleep until FIFO in FPGA is filled
  std::cout << "Waiting . . . " << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(1));

  FILE *outf{nullptr};

  const size_t frame_size = 1920 + 8;
  const size_t buffer_size = 32768 * 4; // FIFO depth 32768 * 4 bytes
  const size_t read_size = 10000;

  char buffer[buffer_size] = {0};
  char *pos = &buffer[0];
  char *used = &buffer[0];
  char *endpos = &buffer[0] + buffer_size;
  size_t bytes_avail = pos - used;

  m_runNumber += 1;

  std::string ssFilename = m_dataTaker->m_filename;
  std::cout << "Filename = " << ssFilename.c_str() << std::endl;
  outf = fopen(ssFilename.c_str(), "wb");

  //add datetime to the beginning of the file by Lu
  time_t t = time(0);
  char tmp[64];
  strftime(tmp, sizeof(tmp), "**********%Y%m%d%H%M%S**********", localtime(&t));
  fwrite(tmp, 33, 1, outf);
  fwrite("\r\n", 2, 1, outf);
  while (!m_stop)
  {
    while (bytes_avail < frame_size)
    {
      if (ferror(fd))
      {
        std::cerr << "ERROR";
        m_dataTaker->reportThreadStopped();
        fclose(fd);
        if (outf)
          fclose(outf);
        return;
      }

      if (pos + read_size > endpos)
      {
        std::cout << "End of Buffer Size" << std::endl;
        // Initialization of pointers
        pos = &buffer[0];
        used = &buffer[0];
        bytes_avail = pos - used;
      }
      if (pos + read_size > endpos)
      {
        // This shouldn't happen, we cannot read past the end of the buffer
        std::cout << "pos: " << (void *)pos << std::endl;
        std::cout << "pos - &buffer[0]: "
                  << (size_t)(pos - &buffer[0]) << std::endl;
        std::cout << "read_size: " << read_size << std::endl;
        std::cout << "endpos: " << (void *)endpos << std::endl;
        std::cout << "bytes_avail: " << bytes_avail << std::endl;
      }

      CHECK(pos + read_size <= endpos);
      CHECK(used <= pos);
      CHECK(pos - used == bytes_avail);
      CHECK(bytes_avail <= buffer_size);

      size_t bytes_got = fread(pos, 1, read_size, fd);
      m_bytesRead += bytes_got;

      std::cout << "readout size (byte) = " << bytes_got << "  ";
      std::cout << "total readout size (byte) = " << m_bytesRead
                << std::endl;

      CHECK(bytes_got > 0);

      pos += bytes_got;
      bytes_avail = pos - used;

      // Save Raw Data to File
      fwrite(used, 1, read_size, outf);

      if (m_stop)
      {
        m_dataTaker->reportThreadStopped();
        fclose(fd);
        if (outf)
          fclose(outf);
        return;
      }
    }

    while (bytes_avail >= frame_size)
    {

      if (used[0] != '\xAA' || used[4 + 1920] != '\xF0')
      {
        // 4(header)+(4byte*10)*48rows
        for (char *offset = used; offset < pos; offset++)
        {
          if (offset[0] == '\xAA' && offset[4 + 1920] == '\xF0')
          {
            std::cout << "Resynchronized! Skipped " << (offset - used) << " bytes" << std::endl;
            // found head/tail
            used = offset;
            bytes_avail = pos - used;
            break;
          }
        }
        if (bytes_avail < frame_size)
        {
          // get more data
          break;
        }
      }

      {
        std::lock_guard<std::mutex> guard(eventDataMutex);
        m_eventNumber++;
        if (m_eventNumber % 100 == 0)
        {
          std::cout << "Event Number = " << m_eventNumber << std::endl;
        }

        if (m_eventNumber % 2000 == 0)
        {
          std::cout << "Read " << m_eventNumber << " events" << std::endl;
          memcpy(m_recentEvent, used, 1928); // Send the entire frame
        }
      }

      if (m_eventNumber > m_dataTaker->get_max_event_number())
      {
        //	DAQ_INFO("Event limit reached.");
        //	DAQ_INFO("Read " << m_eventNumber << " events");
        std::cout << "Event limit reached." << std::endl;
        std::cout << "Read " << m_eventNumber << " events" << std::endl;
        return;
      }

      used += frame_size;
      bytes_avail = pos - used;
    }
  }
  std::cout << "Quit taking data" << std::endl;
  m_dataTaker->reportThreadStopped();
  fclose(fd);
  if (outf)
    fclose(outf);
  return;
}
