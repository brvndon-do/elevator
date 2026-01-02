cmake_minimum_required(VERSION 4.0)

project(ElevatorSim)

set(CMAKE_CXX_STANDARD 23)

include_directories(include/elevator)

set(SOURCES src/main.cpp src/elevator.cpp src/mailbox.cpp)

add_executable(elevator_sim ${SOURCES})

# enable_testing()
# add_subdirectory(tests)