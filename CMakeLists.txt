cmake_minimum_required(VERSION 3.11)
project(pactree)

# build type
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE release)
    message(STATUS "build type set to release")
endif ()
string(TOLOWER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE)
message(STATUS "build type: ${CMAKE_BUILD_TYPE}")

execute_process(COMMAND cat /proc/cpuinfo COMMAND grep clwb OUTPUT_VARIABLE CLWB)
if(CLWB)
    message(STATUS "Use clwb")
else()
        add_definitions(-DNO_CLWB)
    message(STATUS "Use clflush instaed of clwb")
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-pthread -Wall -Wextra -march=native -mavx512bw")


# all the binary will be in the build folder
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include_directories(${CMAKE_SOURCE_DIR}/include)
enable_testing()

add_subdirectory(lib/PDL-ART)
add_subdirectory(src)
add_subdirectory(example)
#add_subdirectory(tests)
#add_subdirectory(benchmarks)
