set(CMAKE_CXX_FLAGS "-Wno-sign-conversion")

aux_source_directory(${CMAKE_SOURCE_DIR}/src kata_srcs)

add_executable(kata_runner
    ${kata_srcs}
)

target_link_libraries(kata_runner
    pthread
    boost_timer
    boost_thread
    boost_program_options
    Boost::log_setup 
    Boost::log
)
