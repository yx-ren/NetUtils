#include <NetUtils/services/io_mode/multi_threads/ThreadsTcp6BroServer.h>
#include <common/base/logger/LoggerManager.h>

using namespace CBASE_NAMESPACE;
using namespace CBASE_LOGGER_NAMESPACE;

NU_SER_BEGIN

bool ThreadsTcp6BroServer::process(IContextSPtr context)
{
    return mClientProcessor->process(context);
}

bool ThreadsTcp6BroServer::run()
{
    while (true)
    {
        struct sockaddr_in6 cli_addr;
        socklen_t ipv6addr_len = sizeof(sockaddr_in6);
        int client_socket = accept(mFd, (struct sockaddr*)&cli_addr, &ipv6addr_len);
        if (client_socket < 0)
        {
            CBT_WARN("ThreadsTcp6BroServer", "run() accept() cause error, "
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

        IPV6SocketContextPtr sock_context(new IPV6SocketContext(client_socket, ntohs(cli_addr.sin6_port), cli_addr));
        char buf[0xff] = {0};
        sock_context->mClientAddrStr = inet_ntop(AF_INET6, &cli_addr.sin6_addr, buf, sizeof(buf));
        CBT_DEBUG("ThreadsTcp6BroServer", "run() new client arrived, accept the connection, "
                "client info:" << sock_context->toString());
        {
            write_lock wlock(mRWLock);
            mSocketContextQueue.push(sock_context);
        }

        if (!process(sock_context))
        {
            CBT_WARN("ThreadsTcp6BroServer", "run() failed to process client io, "
                    "client info:" << sock_context->toString());
        }
    }

    return true;
}

NU_SER_END
