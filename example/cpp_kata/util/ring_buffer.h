/*
 * ring_buffer.h
 *
 *  Created on: Feb 1, 2020
 *      Author: walter
 */
#pragma once

#ifndef UTILS_COMMON_RING_BUFFER_H_
#define UTILS_COMMON_RING_BUFFER_H_

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace wfan {

using namespace std::chrono_literals;

template <typename T>
class RingBuffer {

public:
    RingBuffer(size_t capacity) : m_capacity(capacity), m_size(0), m_head(0), m_tail(0) {
        m_buffer.resize(m_capacity);
    }

    void enqueue(const T& item, const std::chrono::milliseconds& timeout) {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_not_full.wait_for(lock, timeout, [this] { return !is_full(); });

        if (is_full()) {
            // If the buffer is full, overwrite the oldest element
            m_head = (m_head + 1) % m_capacity;
        } else {
            // If there is space, increase the size of the buffer
            m_size++;
        }

        // Add the new item to the tail
        m_buffer[m_tail] = item;
        m_tail = (m_tail + 1) % m_capacity;

        lock.unlock();
        m_not_empty.notify_one(); // Notify potential consumers that the buffer is not empty
    }

    int dequeue(T& result, const std::chrono::milliseconds& timeout) {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_not_empty.wait_for(lock, timeout, [this] { return !is_empty(); });

        if (is_empty()) {
            return -1;
        }

        // Remove and return the item at the head
        result = m_buffer[m_head];
        m_head = (m_head + 1) % m_capacity;
        m_size--;

        lock.unlock();
        m_not_full.notify_one(); // Notify potential producers that the buffer is not full

        return 0;
    }

    std::vector<T> find(const T& min, const T& max) const {
        std::lock_guard<std::mutex> lock(m_mutex);

        std::vector<T> result;
        size_t index = m_head;
        for (size_t i = 0; i < m_size; ++i) {
            if (m_buffer[index] >= min && m_buffer[index] <= max) {
                result.push_back(m_buffer[index]);
            }
            index = (index + 1) % m_capacity;
        }

        return result;
    }

    std::vector<T> find(const T& val) const {
           std::lock_guard<std::mutex> lock(m_mutex);

           std::vector<T> result;
           size_t index = m_head;
           for (size_t i = 0; i < m_size; ++i) {
               if (m_buffer[index] == val) {
                   result.push_back(m_buffer[index]);
               }
               index = (index + 1) % m_capacity;
           }

           return result;
    }

    std::vector<T> get_buffer() const {
        std::lock_guard<std::mutex> lock(m_mutex);

        std::vector<T> result;
        size_t index = m_head;
        for (size_t i = 0; i < m_size; ++i) {
            result.push_back(m_buffer[index]);
            index = (index + 1) % m_capacity;
        }

        return result;
    }
    
    size_t size() const {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_size;
    }

    size_t capacity() const {
        return m_capacity;
    }
protected:
    bool is_empty() const {
        return m_size == 0;
    }

    bool is_full() const {
        return m_size == m_capacity;
    }


private:
    std::vector<T> m_buffer;
    size_t m_capacity;
    size_t m_size;
    size_t m_head;
    size_t m_tail;
    mutable std::mutex m_mutex;
    std::condition_variable m_not_full;
    std::condition_variable m_not_empty;
};

} /* namespace wfan */

#endif /* UTILS_COMMON_RING_BUFFER_H_ */
