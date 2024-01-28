#include <cstring>
#include <string>
#include <iostream>
#include <type_traits>
#include <vector>
#include <string_view>
#include <memory>
#include <iomanip>

struct NetworkMessage
{

    NetworkMessage(std::string_view msg) : m_buffer(nullptr)
    { 

        m_buffer = new char[msg.size()];      // allocate
        m_size =  msg.size();
        memcpy(m_buffer, msg.data(), msg.size()); // populate

        m_data.assign(msg.begin(), msg.end());

    }

    NetworkMessage(const uint8_t* data, std::size_t size) {
        assign(data, size);
    }

    void assign(const uint8_t* data, std::size_t size) {
        m_data.reserve(size);
        m_data.assign(data, data + size);
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


    char* m_buffer; // raw pointer - bad
    size_t m_size; // data size
    std::vector<uint8_t> m_data; // data vector - good
};

inline std::ostream& operator<<(std::ostream& os, const NetworkMessage& msg) {

    os << std::hex << std::setfill('0');
    std::vector<uint8_t>::const_iterator it;

    for (it = msg.m_data.begin(); it != msg.m_data.end(); ++it) {
        os << "\\x" << std::setw(2) << static_cast<unsigned>(*it);
    }

    return os;
}

template<typename T, typename... Ts>
std::unique_ptr<T> make_unique(Ts&&... params) {
    return std::unique_ptr<T>(new T(std::forward<Ts>(params)...));
}

int kata01_buffer(int argc, char** argv) {

    auto msg1 = make_unique<NetworkMessage>("alice");
    auto msg2 = make_unique<NetworkMessage>("tom");

    std::cout << "msg1: "<< *msg1 << ", msg2: "<< *msg2 << std::endl;
    *msg1 = *msg2;
    std::cout << "msg1: "<< *msg1 << ", msg2: "<< *msg2 << std::endl;
    return 0;
}
