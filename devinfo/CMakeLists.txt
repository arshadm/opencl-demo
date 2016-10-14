
project(devinfo)

include_directories("${OPENCL_INCLUDE_DIRS}")

add_executable(devinfo devinfo.cpp)
target_link_libraries(devinfo ${OPENCL_LIBRARIES})
