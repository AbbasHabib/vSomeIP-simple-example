cmake_minimum_required (VERSION 2.8)
project(some_ip_test)
set (CMAKE_CXX_FLAGS "-g -std=c++0x")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

find_library(vsomeip3_lib vsomeip3 
            NAMES vsomeip vsomeip3
            REQUIRED)

add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

add_executable(service-example ./service.cpp)

target_link_libraries(service-example ${vsomeip3_lib} )
