find_package(OpenSSL REQUIRED)

add_executable(log_server log_server.cpp)

target_link_libraries(log_server  ${Boost_LIBRARIES} OpenSSL::SSL OpenSSL::Crypto)

