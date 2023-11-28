project(dpmh)
cmake_minimum_required(VERSION 3.22)

option(LEANSTORE_INCLUDE_XNVME "include xnvme, be carefull as this overrides the used spdk binary to use the one given by xnvme.." OFF) #OFF by default
if(LEANSTORE_INCLUDE_XNVME)
   message("Including XNVME")
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DLEANSTORE_INCLUDE_XNVME")
else()
   # do nothing
endif(LEANSTORE_INCLUDE_XNVME)

include("${CMAKE_SOURCE_DIR}/shared-headers/local.cmake")

include("${CMAKE_SOURCE_DIR}/libs/gflags.cmake")
include("${CMAKE_SOURCE_DIR}/libs/tabluate.cmake")
include("${CMAKE_SOURCE_DIR}/libs/spdk.cmake")
include("${CMAKE_SOURCE_DIR}/libs/uring.cmake")
if(LEANSTORE_INCLUDE_XNVME)
   include("${CMAKE_SOURCE_DIR}/libs/xnvme.cmake")
endif()
include("${CMAKE_SOURCE_DIR}/libs/boost.cmake")



add_subdirectory("io")

add_subdirectory("benchmark")


