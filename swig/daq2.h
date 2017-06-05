
#include <string>
#include <thread>
#include <memory>
#include <atomic>

enum RunState {
    STATE_STOPPED,
    STATE_RUNNING,
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

//class DataTakingThread;


// see https://rafalcieslak.wordpress.com/2014/05/16/c11-stdthreads-managed-by-a-designated-class/
class DataTakingThread {
public:
    DataTakingThread(EventListener* listener):
        m_listener(listener), m_thread(), m_stop(false) {};

    ~DataTakingThread() {
        stopAndJoin();
    }

    void start() {
        // This will start the thread. Notice move semantics!
        m_thread = std::thread(&DataTakingThread::threadMain, this);
    }

    void stopAndJoin() {
        m_stop = true;
        if (m_thread.joinable()) m_thread.join();
    }

private:
    EventListener *m_listener;
    std::thread m_thread;
    std::atomic<bool> m_stop;

    void threadMain();
};


class DataTaker {
public:
    DataTaker(EventListener* listener);
    void start_run();
    void stop_run();
    RunState get_state();
    int get_event_number();
    int get_run_number();
    // get_accumulated_events
    // get_event_number
    // get_run_number
private:
    EventListener* m_listener;
    RunState m_state;
    std::unique_ptr<DataTakingThread> m_threadObj;
};

