#include <NetUtils/services/io_mode/multi_threads/ThreadsTcpBroServer.h>
#include <NetUtils/IOEventLoopThreadPool.h>
#include <common/base/logger/LoggerManager.h>

using namespace CBASE_NAMESPACE;
using namespace CBASE_LOGGER_NAMESPACE;

NU_SER_BEGIN

bool ThreadsTcpBroServer::process(IContextSPtr context)
{
    return mClientProcessor->process(context);
}

bool ThreadsTcpBroServer::run()
{
    while (true)
    {
        struct sockaddr_in cli_addr;
        socklen_t ipv4addr_len = sizeof(sockaddr_in);
        int client_socket = accept(mFd, (sockaddr*)&cli_addr, &ipv4addr_len);
        if (client_socket < 0)
        {
            CBT_WARN("ThreadsTcpBroServer", "run() accept() cause error, "
                "error code: " << errno << ", "
                "engine:" << toString() << " will to exit");
            return false;
        }

#if 0
        // setsockopt(), read from xml
        // TODO......
        struct timeval timeout;
        timeout.tv_sec = 10;
        timeout.tv_usec = 0;

        if (setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO,
            (char *)&timeout, sizeof(timeout)) < 0)
        {
            std::cerr << "server: call setsockopt() failed, for set SO_RCVTIMEO"
                << "err code:[" << GET_LAST_SOCKET_ERROR << "]" << std::endl;
            return false;
        }

        if (setsockopt(client_socket, SOL_SOCKET, SO_SNDTIMEO,
            (char *)&timeout, sizeof(timeout)) < 0)
        {
            std::cerr << "server: call setsockopt() failed, for set SO_SNDTIMEO"
                << "err code:[" << GET_LAST_SOCKET_ERROR << "]" << std::endl;
            return false;
        }
#endif

        // alloc socket context
        IPV4SocketContextSPtr sock_context(new IPV4SocketContext(client_socket, ntohs(cli_addr.sin_port), cli_addr));
        sock_context->mClientAddrStr = inet_ntoa(sock_context->mClientAddr.sin_addr);
        CBT_DEBUG("ThreadsTcpBroServer", "run() new client arrived, accept the connection, "
                "client info:" << sock_context->toString());

        // alloc socket buffer context and push the socket into eventloop, this process user read/write request
        SocketBufferContextSPtr sock_buffer(sock_context);
        IOEventLoopThreadPool::getInstance()->process(sock_buffer);

        // bro processor, this process read/write on this socket
        if (!process(sock_buffer))
        {
            CBT_WARN("ThreadsTcpBroServer", "run() failed to process client io, "
                    "client info:" << sock_buffer()->getSocketContext()->toString());
        }

        // notify the user the new client arrived and register async callback if necessary
        if (mHandleNewClientCb)
            mHandleNewClientCb(sock_buffer);
    }

    return true;
}

NU_SER_END
