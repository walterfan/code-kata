#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <stdexcept>

template <typename T>
class BlockingQueue {
public:
    BlockingQueue(size_t capacity) : m_capacity(capacity) {}

    int enqueue(const T& item, const std::chrono::milliseconds& timeout) {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (m_not_full.wait_for(lock, timeout, [this] 
            { return m_queue.size() < m_capacity; })) {
            m_queue.push(item);
            lock.unlock();
            m_not_empty.notify_one();
            return 0; 
        } else {
            return -1; 
        }
    }

    int dequeue(T& result, const std::chrono::milliseconds& timeout) {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (m_not_empty.wait_for(lock, timeout, [this] { return !m_queue.empty(); })) {
            result = m_queue.front();
            m_queue.pop();
            lock.unlock();
            m_not_full.notify_one();
            return 0; 
        } else {
            return -1; 
        }
    }

private:
    std::queue<T> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_not_empty;
    std::condition_variable m_not_full;
    size_t m_capacity;
};
