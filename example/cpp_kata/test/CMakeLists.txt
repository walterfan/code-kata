set(CMAKE_CXX_FLAGS "-Wno-old-style-cast")
set(CMAKE_CXX_FLAGS "-Wno-unused-variable")
set(CMAKE_CXX_FLAGS "-Wno-sign-conversion")

add_executable(hfrtc hfrtc.cpp)
target_link_libraries(hfrtc tinyutil pthread)

add_executable(admin admin.cpp)
target_link_libraries(admin tinyutil pthread)

add_executable(uas uas.cpp)
target_link_libraries(uas tinyutil pthread)

if(LINUX)
    message(STATUS "Linux support inotify")
    add_executable(file_monitor file_monitor.cpp)
else()
    message(STATUS "Not Linux with inotify")
endif()

add_executable(unit_test UnitTest.cpp
    kata_helper_test.cpp
    time_util_test.cpp
    server_test.cpp 
    ring_buffer_test.cpp
    TinyUtilTest.cpp  
    TinyConfigTest.cpp)
target_link_libraries(unit_test tinyutil kata_util gtest)

add_executable(complaint-server complaint-server.cpp)
add_executable(complaint-client complaint-client.cpp)

add_executable(waltertest waltertest.cpp)
target_link_libraries(waltertest tinyutil)