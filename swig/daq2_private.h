
#include <atomic>
#include <thread>

class EventListener;

// see https://rafalcieslak.wordpress.com/2014/05/16/c11-stdthreads-managed-by-a-designated-class/
class DataTakingThread {
public:
    DataTakingThread(DataTaker *dataTaker):
        m_dataTaker(dataTaker), m_listener(dataTaker->m_listener), m_thread(), m_stop(false), m_eventNumber(0) {};

    ~DataTakingThread() {
        stopAndJoin();
    }

    void start() {
        // This will start the thread. Notice move semantics!
        m_thread = std::thread(&DataTakingThread::threadMain, this);
    }

	void stop() {
		m_stop = true;
	}

    void stopAndJoin() {
        m_stop = true;
        if (m_thread.joinable()) m_thread.join();
    }

    std::atomic<int> m_eventNumber;

private:
	DataTaker *m_dataTaker;
    EventListener *m_listener;
    std::thread m_thread;
    std::atomic<bool> m_stop;

    void threadMain();
};
