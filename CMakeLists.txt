cmake_minimum_required(VERSION 3.10)
project(simio)

# enable c++ 11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS_DEBUG_INIT "-std=c++11 -g -fno-omit-frame-pointer")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-std=c++11 -g -O2")

# default build type : Debug
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif ()

# INCLUDE for C/C++ include
set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_INCLUDE_DIR})

add_subdirectory(${PROJECT_SOURCE_DIR}/simio)

add_subdirectory(${PROJECT_SOURCE_DIR}/example)

enable_testing()
add_subdirectory(${PROJECT_SOURCE_DIR}/test)
