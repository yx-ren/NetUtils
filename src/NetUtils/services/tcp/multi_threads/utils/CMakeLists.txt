#file(GLOB SOURCE_FILE
#    ThreadsEcho.cpp
#    ThreadsClientHandle.*
#    IPV4ThreadsTcpServer.*
#    IPV6ThreadsTcpServer.*
#    ../TcpServer.*
#    )

file(GLOB COMMON_SOURCE_FILE
    ${PROJECT_SOURCE_DIR}/src/NetUtils/*.cpp
    )

file(GLOB SERVICE_SOURCE_FILE
    ${PROJECT_SOURCE_DIR}/src/NetUtils/services/*.cpp
    )

file(GLOB SERVICE_TCP_SOURCE_FILE
    ${PROJECT_SOURCE_DIR}/src/NetUtils/services/tcp/ITcpBrokerServerEngine.cpp
    )

file(GLOB SERVICE_TCP_MULTI_THREADS_SOURCE_FILE
    ${PROJECT_SOURCE_DIR}/src/NetUtils/services/tcp/multi_threads/ThreadsTcpBroServer.*
    ${PROJECT_SOURCE_DIR}/src/NetUtils/services/tcp/multi_threads/ThreadsTcp6BroServer.*
    ${PROJECT_SOURCE_DIR}/src/NetUtils/services/tcp/multi_threads/ThreadsTcpBroProcessor.*
    ${PROJECT_SOURCE_DIR}/src/NetUtils/services/tcp/multi_threads/ThreadsTcpBroWorker.*
    )

add_executable(echo_server_threads
    ThreadsEchoEngine.cpp
    ${COMMON_SOURCE_FILE}
    ${SERVICE_SOURCE_FILE}
    ${SERVICE_TCP_SOURCE_FILE}
    ${SERVICE_TCP_MULTI_THREADS_SOURCE_FILE}
    )

#add_executable(echo_server_threads
#    ThreadsEchoEngine.cpp
#    )
if (WIN32)
    target_link_libraries(echo_server_threads ws2_32.lib)
endif()
target_link_libraries(echo_server_threads ${Log4CXX_LIBRARIES})
target_link_libraries(echo_server_threads ${Boost_LIBRARIES})
target_link_libraries(echo_server_threads cbase)

#SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
