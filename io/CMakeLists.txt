file(GLOB_RECURSE LEANSTORE_CC **.cpp **/**.cpp **.hpp **/**.hpp)
if (NOT UNIX)
    message(SEND_ERROR "unsupported platform")
endif ()

add_library(io STATIC ${LEANSTORE_CC})
target_link_libraries(io gflags tbb aio)
add_dependencies(io liburing)
add_dependencies(io spdk)
target_include_directories(io PUBLIC ${SHARED_INCLUDE_DIRECTORY})


target_link_libraries(io -L${SPDK_LIBRARY_PATH})
target_link_libraries(io  -lspdk_vmd -Wl,--whole-archive -lspdk_nvme -Wl,--no-whole-archive -lspdk_trace -lspdk_sock -lspdk_log -lspdk_json -lspdk_env_dpdk -lspdk_util -lspdk_vfio_user)
target_link_libraries(io
	-Wl,--whole-archive
	${DPDK_LIBRARY_PATH}/librte_eal.a
	${DPDK_LIBRARY_PATH}/librte_telemetry.a
	${DPDK_LIBRARY_PATH}/librte_mempool.a
	${DPDK_LIBRARY_PATH}/librte_kvargs.a
	${DPDK_LIBRARY_PATH}/librte_bus_pci.a
	${DPDK_LIBRARY_PATH}/librte_pci.a
	${DPDK_LIBRARY_PATH}/librte_ring.a
	-Wl,--no-whole-archive
)
target_link_libraries(io -luuid -lnuma -lrt -lcrypto -laio)
target_link_libraries(io -L${ISAL_LIBRARY_PATH} -lisal )

target_link_libraries(io ${TBB_LIBRARIES})

target_link_libraries(io ${Boost_LIBRARIES} )
#target_link_libraries(io -L~/boost_1_73_0/stage/lib/ -lboost_context)
#target_include_directories(io PUBLIC ~/boost_1_73_0/boost/context/)
# use it with cmake ... -DCMAKE_CXX_FLAGS='-Dleanstore_USE_TASKING -DBOOST_USE_UCONTEXT'

target_link_libraries(io -L${liburing_LIBRARY_PATH})
target_link_libraries(io -luring )

target_link_libraries(io ${CMAKE_DL_LIBS})
