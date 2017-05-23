FILE(GLOB IPV4
    IPV4ThreadsEcho.cpp
    ThreadsTcpServer.*
    IPV4ThreadsTcpServer.*
    ../TcpServer.*
    )
ADD_EXECUTABLE(echo_server_threads ${IPV4})
#SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
