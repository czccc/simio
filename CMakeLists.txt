cmake_minimum_required(VERSION 3.10)
project(simio)


## set output path
#set(PROJECT_OUTPUT_DIR ${PROJECT_BINARY_DIR}/${CMAKE_SYSTEM_PROCESSOR})
#set(PROJECT_INCLUDE_DIR ${PROJECT_OUTPUT_DIR}/include)
#
#message("-- system arch:  ${CMAKE_SYSTEM_PROCESSOR}")
#message("-- output path:  ${PROJECT_OUTPUT_DIR} ")
#
## mkdir directory
#file(MAKE_DIRECTORY ${PROJECT_INCLUDE_DIR})
#file(MAKE_DIRECTORY ${PROJECT_OUTPUT_DIR}/bin)
#file(MAKE_DIRECTORY ${PROJECT_OUTPUT_DIR}/lib)
#
### set the target path
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_OUTPUT_DIR}/bin)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_OUTPUT_DIR}/lib)
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_OUTPUT_DIR}/lib)


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

enable_testing()
add_subdirectory(${PROJECT_SOURCE_DIR}/test)
