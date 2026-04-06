set(CMAKE_CXX_FLAGS "-Wno-sign-conversion")

aux_source_directory(${CMAKE_SOURCE_DIR}/src kata_srcs)
list(REMOVE_ITEM kata_srcs "kata_runner.cpp")


set(KATA_DEPS
    pthread
    boost_timer
    boost_thread
    boost_program_options
    Boost::log_setup
    Boost::log
)

add_executable(kata_runner
    ${kata_srcs}
    kata_runner.cpp
)

add_library(kata_util
    ${kata_srcs}
)

target_link_libraries(kata_runner
    ${KATA_DEPS}
)

target_link_libraries(kata_util
    ${KATA_DEPS}
)

