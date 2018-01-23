
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
	void set_simulate_state(int sim) { m_simulate = sim; };
	int get_simulate_state() { return m_simulate; };
    void set_max_event_number(int enumber) {m_max_event_number=enumber;};
    int get_max_event_number() {return m_max_event_number;};
    void re_set();
    // get_accumulated_events
    // get_event_number
    // get_run_number

	void get_recent_event(char *data);
	
	const std::string& get_filename(){ return m_filename; }
	void set_filename( const std::string &fname ){ m_filename = fname; }

private:
    EventListener* m_listener;
    std::atomic<RunState> m_state;
    std::unique_ptr<DataTakingThread> m_threadObj;

	void reportThreadStopped();
	std::string m_filename;
    int m_simulate;
    int m_max_event_number;
};
