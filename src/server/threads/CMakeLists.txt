FILE(GLOB SOURCE_FILE
    IPV4ThreadsEcho.cpp
    ThreadsClientHandle.*
    IPV4ThreadsTcpServer.*
    IPV6ThreadsTcpServer.*
    ../TcpServer.*
    )
ADD_EXECUTABLE(echo_server_threads ${SOURCE_FILE})
TARGET_LINK_LIBRARIES(echo_server_threads ws2_32.lib)
#SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
