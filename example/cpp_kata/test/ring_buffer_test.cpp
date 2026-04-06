/*
 * RingBuffer_test.cpp
 *
 *  Created on: Jul 4, 2016
 *      Author: walter
 */

#include <gtest/gtest.h>
#include <string>
#include <iostream>
#include <memory>
#include "ring_buffer.h"

using namespace std;
using namespace testing;
using namespace wfan;

class RingBufferTest: public Test {
public:
	RingBufferTest(): m_ring_buffer(nullptr) {
		m_ring_buffer = std::make_unique<RingBuffer<std::string>>(5);
    }

    void SetUp( ) {
       m_retrieved.reserve(10);
    }

    void TearDown( ) {
       m_retrieved.clear();
    }

    void producer() {
        for (int i = 0; i < 7; ++i) {
            m_ring_buffer->enqueue(std::to_string(i), std::chrono::milliseconds(20));
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    void consumer() {
        for (int i = 0; i < 7; ++i) {
            std::string item;
            int result = m_ring_buffer->dequeue(item, std::chrono::milliseconds(20));
            if (result == 0) {
                std::cout << "Consumed: " << item << std::endl;
            } else if (result < 0) {
                std::cout << "Dequeue failed with timeout" << std::endl;
            }
            m_retrieved.push_back(item);
        }
    }

    std::unique_ptr<RingBuffer<std::string>> m_ring_buffer;
    std::vector<std::string> m_retrieved;

};

TEST_F (RingBufferTest, enqueue)
{
    RingBuffer<int> ringBuffer(5);

    // Enqueue elements in a separate thread
    std::thread producer([&]() {
        for (int i = 1; i <= 7; ++i) {
            ringBuffer.enqueue(i, std::chrono::milliseconds(20));
            std::cout << "Enqueued: " << i << ", Size: " << ringBuffer.size() << std::endl;
        }
    });

    producer.join(); // Wait for the producer thread to finish
    std::cout << "Elements: ";
    auto elements = ringBuffer.get_buffer();
    for (const auto& element : elements) {
        std::cout << element << ", ";
    }
   
    ASSERT_TRUE(std::find(elements.begin(), elements.end(), 1) == elements.end());
    ASSERT_TRUE(std::find(elements.begin(), elements.end(), 2) == elements.end());
    // Find elements between 2 and 5
    std::vector<int> result = ringBuffer.find(2, 5);
    ASSERT_TRUE(result.size() == 3);
    // Display the found elements
    std::cout << "\nElements between 2 and 5: ";
    for (const auto& value : result) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
    ASSERT_TRUE(std::find(elements.begin(), elements.end(), 3) != elements.end());
   
}

TEST_F (RingBufferTest, dequeue)
{
    std::thread producerThread([&]() { producer();});
    std::thread consumerThread([&]() { consumer();});

    producerThread.join();
    consumerThread.join();

    ASSERT_TRUE(m_retrieved.size() == 7);
    ASSERT_TRUE(std::find(m_retrieved.begin(), m_retrieved.end(), "6") 
        != m_retrieved.end());
}
