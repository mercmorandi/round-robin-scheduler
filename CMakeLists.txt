cmake_minimum_required(VERSION 3.14)
project(round_robin_scheduler)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(round_robin_scheduler main.cpp Job.cpp Job.h JobScheduler.cpp JobScheduler.h Worker.cpp Worker.h)