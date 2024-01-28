set(CMAKE_CXX_FLAGS "-Wno-old-style-cast")
set(CMAKE_CXX_FLAGS "-Wno-unused-variable")
set(CMAKE_CXX_FLAGS "-Wno-sign-conversion")

add_executable(hfrtc hfrtc.cpp)
target_link_libraries(hfrtc tinyutil pthread)

add_executable(admin admin.cpp)
target_link_libraries(admin tinyutil pthread)

add_executable(uas uas.cpp)
target_link_libraries(uas tinyutil pthread)


add_executable(complaint-server complaint-server.cpp)
add_executable(complaint-client complaint-client.cpp)


add_executable(kata_runner
    kata_runner.cpp
    kata01_buffer.cpp
    kata02_fib.cpp
    kata03_crtp.cpp
    kata04_logger.cpp
    kata05_coin_change.cpp
    kata06_subset.cpp
    kata07_permute.cpp
    kata13_line_counter.cpp
)

target_link_libraries(kata_runner
    pthread
    boost_timer
    boost_thread
    boost_program_options
    Boost::log_setup 
    Boost::log
)
