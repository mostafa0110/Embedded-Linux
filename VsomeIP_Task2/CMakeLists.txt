cmake_minimum_required(VERSION 3.10)
project(CPP_Task5)

set(CMAKE_CXX_STANDARD 17)

find_package(vsomeip3 3.1.20 REQUIRED)
find_package(Boost REQUIRED COMPONENTS system thread)

add_library(GpioLib src/gpio.cpp)
target_include_directories(GpioLib PUBLIC include)

add_executable(rpi_server src/rpi_server.cpp)
target_link_libraries(rpi_server vsomeip3 Boost::system Boost::thread GpioLib)


add_executable(laptop_client src/laptop_client.cpp)
target_include_directories(laptop_client PUBLIC include)
target_link_libraries(laptop_client vsomeip3 Boost::system Boost::thread)