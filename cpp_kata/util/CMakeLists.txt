set(CMAKE_CXX_FLAGS "-Wno-old-style-cast")
set(CMAKE_CXX_FLAGS "-Wno-unused-variable")
set(CMAKE_CXX_FLAGS "-Wno-sign-conversion")

if(APPLE)
include_directories(/opt/homebrew/Cellar/gd/2.3.3_6/include/)
endif()

set(TinyUtil_SRCS 
    TinyUtil.cpp 
    TinyConfig.cpp 
    TinyTimer.cpp  
    TinyServer.cpp  
    TinyClient.cpp  
    TinyRio.cpp
    string_util.cpp
    time_util.cpp
)

add_library(tinyutil ${TinyUtil_SRCS})

target_link_libraries(tinyutil pthread)