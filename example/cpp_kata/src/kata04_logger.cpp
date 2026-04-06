#include <condition_variable>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <type_traits>
#include <vector>
#include <mutex>
#include <thread>
#include <memory>
#include <queue>
#include <atomic>
#include <chrono>

class Logger {
public:
    Logger();
    virtual ~Logger();
    Logger(const Logger& rhs) = delete;
    Logger& operator=(const Logger& rhs) = delete;
    void log(std::string msg);
    void stop();
private:
    void processLogs();
    void processLogsHelper(std::queue<std::string>& queue, std::ofstream& ofs);
    bool isStopped();
    std::mutex m_mutex;
    std::condition_variable m_condtion;
    std::queue<std::string> m_queue;
    std::thread m_thread;
    std::atomic<bool> m_stopped;
    bool m_stdout;
};

Logger::Logger(): m_stopped(false), m_stdout(true) {
    m_thread = std::thread{&Logger::processLogs, this};

}

Logger::~Logger() {
    stop();
    std::unique_lock<std::mutex> lock{m_mutex};
    m_condtion.notify_all();
    m_thread.join();

}

bool Logger::isStopped() {
    return m_stopped.load();
}

void Logger::stop() {
    if (m_stopped.load()) {
        return;
    }
    m_stopped = true;
    std::cout << "stop thread " << m_stopped.load() << std::endl;
    //m_stopped.notify_all();
}

void Logger::log(std::string msg) {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_queue.push(std::move(msg));
    m_condtion.notify_all();
}

void Logger::processLogs() {
    std::string file_name{"kata.log"};
    std::ofstream log_file(file_name.c_str());
    if (log_file.fail()) {
        std::cerr << "Failed to open " << file_name << std::endl;
        return;
    }
    std::cout << "started thread" << std::endl;
    std::unique_lock<std::mutex> lock{m_mutex, std::defer_lock};

    while(true) {
        lock.lock();
        if(isStopped()) {
            processLogsHelper(m_queue, log_file);
            break;
        }

        m_condtion.wait(lock);

        std::queue<std::string> local_queue;
        local_queue.swap(m_queue);
        lock.unlock();

        processLogsHelper(local_queue, log_file);
    }
}

void Logger::processLogsHelper(std::queue<std::string>& queue, std::ofstream& ofs) {
    while(!queue.empty()) {
        ofs << queue.front() << std::endl;
        if (m_stdout) {
            std::cout << queue.front() << std::endl;
        }

        queue.pop();
    }
}

void write_log_demo(int id, Logger& logger) {
    for(int i=0; i< 10; ++i) {
        std::ostringstream oss;
        oss << i << ". log at " << __FILE__ << "" << __LINE__
            << " from thread " << id;
        logger.log(oss.str());

    }
}

int kata04_logger(int argc, char** argv) {
     using namespace std::chrono_literals;

    Logger logger;
    std::vector<std::thread> threads;

    for(int i=0; i< 10; ++i) {
        threads.emplace_back(write_log_demo, i, std::ref(logger));
    }

    std::this_thread::sleep_for(1000ms);
    logger.stop();
    for(auto& thread: threads) {
        thread.join();
    }
    return 0;
}
