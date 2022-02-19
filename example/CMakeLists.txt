
set(BSRCS benchmark.cpp)
set(BBIN bench_pactree)
add_executable(${BBIN} ${BSRCS})
target_include_directories(${BBIN} PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(${BBIN} pactree pdlart tbb numa pmem pmemobj gflags)


set(ESRCS example.cpp)
set(EBIN pactree-example)
add_executable(${EBIN} ${ESRCS})
target_include_directories(${EBIN} PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(${EBIN} pactree pdlart tbb numa pmem pmemobj gflags)

