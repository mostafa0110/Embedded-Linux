cmake_minimum_required(VERSION 3.10)
project(SevenSegmentProject)



add_library(SevenSegmentLib SHARED
    src/Stream.cpp
    src/IStream.cpp
    src/OStream.cpp
    src/SevenSegment.cpp
    src/TerminalStream.cpp
    src/GPIO.cpp
)
target_include_directories(SevenSegmentLib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_executable(MyApp app/main.cpp)
target_link_libraries(MyApp PRIVATE SevenSegmentLib)


set(RPI_USER "mostafa")
set(RPI_IP   "10.42.0.212")
set(RPI_PATH "/home/mostafa/")


add_custom_target(deploy
    COMMAND scp $<TARGET_FILE:MyApp> ${RPI_USER}@${RPI_IP}:${RPI_PATH}
    
    COMMAND scp $<TARGET_FILE:SevenSegmentLib> ${RPI_USER}@${RPI_IP}:${RPI_PATH}

    COMMAND echo "Successfully deployed to ${RPI_IP}"
    
    DEPENDS MyApp SevenSegmentLib
)