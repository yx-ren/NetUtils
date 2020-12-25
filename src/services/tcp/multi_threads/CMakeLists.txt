file(GLOB SOURCE_FILE
    ThreadsEcho.cpp
    ThreadsClientHandle.*
    IPV4ThreadsTcpServer.*
    IPV6ThreadsTcpServer.*
    ../TcpServer.*
    )
add_executable(echo_server_threads ${SOURCE_FILE})
if (WIN32)
    target_link_libraries(echo_server_threads ws2_32.lib)
endif()
target_link_libraries(echo_server_threads log4cxx)
target_link_libraries(echo_server_threads cbase)
#SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
