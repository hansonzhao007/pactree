file(GLOB artsrc "*.cpp" "*.h")

add_library(pdlart STATIC ${artsrc})

target_include_directories(pdlart PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

INSTALL(TARGETS pdlart 
        ARCHIVE DESTINATION ${CMAKE_SOURCE_DIR}
        )

