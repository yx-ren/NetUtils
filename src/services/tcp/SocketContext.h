#ifndef __SOCKET_CONTEXT_H
#define __SOCKET_CONTEXT_H

#if defined(WIN32) || defined (_WIN32)
#include <ws2tcpip.h>
#include <WinSock.h>
#include <process.h>
#include <mstcpip.h>
#else
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

class SocketContext
{
public:
    explicit SocketContext(int fd)
        : mClientFd(fd)
    {}

    virtual ~SocketContext()
    {}

    int mClientFd;
};
typedef std::shared_ptr<SocketContext> SocketContextPtr;

class IPV4SocketContext : public SocketContext
{
public:
    IPV4SocketContext(const struct sockaddr_in& addr, int fd)
        : SocketContext(fd)
        , mClientAddr(addr)
    {}

	~IPV4SocketContext()
	{}

    struct sockaddr_in mClientAddr;
};
typedef std::shared_ptr<IPV4SocketContext> IPV4SocketContextPtr;

class IPV6SocketContext : public SocketContext
{
public:
    IPV6SocketContext(const struct sockaddr_in6& addr, int fd)
        : SocketContext(fd)
        , mClientAddr(addr)
    {}

	~IPV6SocketContext()
	{}

    struct sockaddr_in6 mClientAddr;
};
typedef std::shared_ptr<IPV6SocketContext> IPV6SocketContextPtr;

#endif
