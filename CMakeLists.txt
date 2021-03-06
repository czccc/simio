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


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lpthread")

# INCLUDE for C/C++ include
set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_INCLUDE_DIR})

add_subdirectory(${PROJECT_SOURCE_DIR}/simio)

add_subdirectory(${PROJECT_SOURCE_DIR}/test)

link_directories(${PROJECT_SOURCE_DIR}/simio)

# create executable-file
# add_executable(exe-filename needed-C_or_C++file)

# create shared-library
# add_library(lib-filename SHARED needed-files-list)

