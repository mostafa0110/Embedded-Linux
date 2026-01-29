
cmake_minimum_required(VERSION 3.10)

project(CapsLockControl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS system thread)
find_package(vsomeip3 REQUIRED)

include_directories(src)


add_executable(caps_server 
    src/server.cpp 
)
target_link_libraries(caps_server
    vsomeip3
    pthread
)




add_executable(caps_client 
    src/client.cpp
)
target_link_libraries(caps_client
    vsomeip3
    pthread
)