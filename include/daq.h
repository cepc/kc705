
#include <cstring>
#include <string>
#include <thread>
#include <memory>
#include <atomic>

enum RunState {
    STATE_STOPPED,
    STATE_RUNNING,
	// stop command issued, but thread hasn't stopped yet (e.g. hanging on read)
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

class DataTakingThread;

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
    // get_accumulated_events
    // get_event_number
    // get_run_number

	void getRecentEvent(char *data);

private:
    EventListener* m_listener;
    std::atomic<RunState> m_state;
    std::unique_ptr<DataTakingThread> m_threadObj;

	void reportThreadStopped();
};
