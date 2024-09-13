cmake_minimum_required(VERSION 3.10)

project(kata_cpp C CXX)

enable_testing()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CXX_FLAGS
 -g
 # -DVALGRIND
 # -DMUDUO_STD_STRING
 -DCHECK_PTHREAD_RETURN_VALUE
 -D_FILE_OFFSET_BITS=64
 -Wall
 -Wextra
 -Wno-old-style-cast
 -Wno-unused-variable
 -Wno-sign-conversion
 -Wdeprecated-copy
 #-Werror
 -Wconversion
 -Wno-unused-parameter
 -Wold-style-cast
 -Woverloaded-virtual
 -Wpointer-arith
 -Wshadow
 -Wwrite-strings
 -march=native
 # -MMD
 -std=c++20
# -stdlib=libc++
 #-rdynamic
 )

if(CMAKE_BUILD_BITS EQUAL 32)
  list(APPEND CXX_FLAGS "-m32")
endif()

string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

if(UNIX AND NOT APPLE)
  set(CMAKE_CXX_COMPILER "g++-10")
endif()


set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(CMAKE_CXX_STANDARD 20)

SET(Boost_USE_STATIC_LIBS ON)
SET(Boost_USE_MULTITHREAD ON)

#add_compile_definitions(BOOST_LOG_DYN_LIN)
#ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)

find_package(Boost REQUIRED COMPONENTS system thread regex log log_setup program_options asio beast)

find_package(CURL)
find_package(ZLIB)
find_package(GTest)

find_path(GMOCK_INCLUDE_DIR gmock/gmock.h)
find_library(GMOCK_LIBRARY NAMES gmock)

find_path(CARES_INCLUDE_DIR ares.h)
find_library(CARES_LIBRARY NAMES cares)
find_path(MHD_INCLUDE_DIR microhttpd.h)
find_library(MHD_LIBRARY NAMES microhttpd)
find_library(BOOSTTEST_LIBRARY NAMES boost_unit_test_framework)
find_library(BOOSTPO_LIBRARY NAMES boost_program_options)
find_path(TCMALLOC_INCLUDE_DIR gperftools/heap-profiler.h)
find_library(TCMALLOC_LIBRARY NAMES tcmalloc_and_profiler)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/util)
include_directories(${PROJECT_SOURCE_DIR}/src)

link_directories(/usr/local/lib)

string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
message(STATUS "CXX_FLAGS = " ${CMAKE_CXX_FLAGS} " " ${CMAKE_CXX_FLAGS_${BUILD_TYPE}})


if (APPLE)
    set(GST_INCLUDE_DIRS /Library/Frameworks/GStreamer.framework/Headers)
    set(GST_LIBRARY_DIRS /Library/Frameworks/GStreamer.framework/Versions/Current/lib)

    include_directories(${GST_INCLUDE_DIRS})
    link_directories(${GST_LIBRARY_DIRS})

    set(GTEST_INCLUDE_DIRS /opt/homebrew/Cellar/googletest/1.14.0/include)
    set(GTEST_LIBRARY_DIRS /opt/homebrew/Cellar/googletest/1.14.0/lib)
    include_directories(${GTEST_INCLUDE_DIRS})
    link_directories(${GTEST_LIBRARY_DIRS})

endif()

add_subdirectory(src)
add_subdirectory(util)
add_subdirectory(test)
add_subdirectory(exam)



