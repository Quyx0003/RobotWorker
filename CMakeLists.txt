cmake_minimum_required(VERSION 3.5)

project(
    RobotWorker
    VERSION 1.0
    LANGUAGES CXX
)

find_package(Qt5 REQUIRED COMPONENTS Core Quick Sql)
if(NOT Qt5_FOUND)
    message(FATAL_ERROR "Qt5 not found! Please install Qt5.")
endif()
    
add_executable(RobotWorker main.cpp)
target_sources(RobotWorker PRIVATE client.cpp client.h)
target_sources(RobotWorker PRIVATE robot.cpp robot.h)

target_link_libraries(RobotWorker Qt5::Core Qt5::Quick Qt5::Sql)

