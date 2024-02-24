add_executable(UnitTest UnitTest.cpp 
    time_util_test.cpp
    server_test.cpp 
    ring_buffer_test.cpp
    TinyUtilTest.cpp  
    TinyConfigTest.cpp)
target_link_libraries(UnitTest tinyutil gtest)

add_executable(waltertest waltertest.cpp)
target_link_libraries(waltertest tinyutil)