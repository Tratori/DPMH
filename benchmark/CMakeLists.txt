file(GLOB_RECURSE LEANSTORE_CC **.cpp **/**.cpp **.hpp **/**.hpp)
if (NOT UNIX)
    message(SEND_ERROR "unsupported platform")
endif ()

add_library(benchmark STATIC ${LEANSTORE_CC})
target_link_libraries(benchmark gflags io)


add_executable(benchmark_latency benchmark_latency.cpp)
target_link_libraries(benchmark_latency benchmark gflags io)