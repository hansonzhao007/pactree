
cmake_minimum_required(VERSION 3.11)
project(ARTSynchronized)

if( NOT CMAKE_BUILD_TYPE )
    message(STATUS "No build type selected, default to Release")
    set( CMAKE_BUILD_TYPE Release)
else()
    message(STATUS "Build type is set to ${CMAKE_BUILD_TYPE}")
endif()

file(GLOB artsrc "*.cpp" "*.h")
add_library(pdlart STATIC ${artsrc})

target_include_directories(pdlart PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

INSTALL(TARGETS pdlart 
        ARCHIVE DESTINATION ${CMAKE_SOURCE_DIR}
        )

