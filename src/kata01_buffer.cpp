#include <string>
#include <iostream>
#include <vector>
#include <string_view>
#include <memory>

using namespace std;

class NetworkMessage
{
public:
    explicit NetworkMessage(std::string_view msg) : m_buffer(nullptr)
    { 

        m_buffer = new char[msg.size()];      // allocate
        std::memcpy(m_buffer, msg.data(), msg.size()); // populate

        m_data.assign(msg.begin(), msg.end());

    }
 
    ~NetworkMessage()
    {
        delete[] m_buffer; // deallocate
    }
 
    NetworkMessage(const NetworkMessage& other) // copy constructor
        : NetworkMessage(other.m_buffer) {
            m_data.assign(other.m_data.begin(), other.m_data.end());
        }
 
    NetworkMessage(NetworkMessage&& other) noexcept // move constructor
        : m_buffer(std::exchange(other.m_buffer, nullptr)) {
            m_data.swap(other.m_data);
        }
 
    NetworkMessage& operator=(const NetworkMessage& other) // copy assignment
    {
        return *this = NetworkMessage(other);
    }
 
    NetworkMessage& operator=(NetworkMessage&& other) noexcept // move assignment
    {
        std::swap(m_buffer, other.m_buffer);
        m_data.swap(other.m_data);
        return *this;
    }
 
// alternatively, replace both assignment operators with 
//  NetworkMessage& operator=(NetworkMessage other) noexcept
//  {
//      std::swap(m_buffer, other.m_buffer);
//      return *this;
//  }


    char* m_buffer; // raw pointer as dynamically-allocated memory block
    std::vector<uint8_t> m_data;
};


int kata01_buffer(int argc, char** argv) {

    cout << "start "<< endl;
    auto msg1 = std::make_unique<NetworkMessage>("alice");
    auto msg2 = std::make_unique<NetworkMessage>("tom");

    *msg1 = *msg2;
    cout << "msg1: "<< msg1 << "msg2: "<< msg2 << endl;
    return 0;
}