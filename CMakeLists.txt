cmake_minimum_required(VERSION 3.2)

project(opencl-demo)

set(CMAKE_VERBOSE_MAKEFILE on )
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

find_package(OpenCL)

if (NOT OPENCL_FOUND) 
  message(FATAL_ERROR "OpenCL library not found.")
endif ()

add_subdirectory(devinfo)
