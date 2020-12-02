FILE(GLOB SOURCE_FILE
    ThreadsEcho.cpp
    ThreadsClientHandle.*
    IPV4ThreadsTcpServer.*
    IPV6ThreadsTcpServer.*
    ../TcpServer.*
    )
ADD_EXECUTABLE(echo_server_threads ${SOURCE_FILE})
IF (WIN32)
    TARGET_LINK_LIBRARIES(echo_server_threads ws2_32.lib)
ENDIF()
TARGET_LINK_LIBRARIES(echo_server_threads log4cxx)
#SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
