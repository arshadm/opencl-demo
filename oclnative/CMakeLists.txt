cmake_minimum_required(VERSION 3.6)
project(oclnative)

set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    src/main/cpp/oclnative.cpp
    src/main/cpp/oclnative.h)

find_package(OpenCL REQUIRED)
find_package(CLBLAS REQUIRED)

add_library(oclnative SHARED ${SOURCE_FILES})
