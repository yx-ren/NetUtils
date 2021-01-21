#ifndef __NET_UTILS_SOCKET_CONTEXT_H__
#define __NET_UTILS_SOCKET_CONTEXT_H__

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
#include <mutex>
#include <vector>
#include <queue>
#include <NetUtils/common.h>
#include <NetUtils/IContext.h>

NU_BEGIN

#define INVALID_FD              -1

class SocketContext : public IContext
{
public:
    SocketContext()
        : mClientFd(INVALID_FD)
    {}

    SocketContext(int fd, int port)
        : mClientFd(fd)
        , mPort(port)
    {}

    virtual ~SocketContext()
    {
        if (mClientFd != INVALID_FD)
            close(mClientFd);
    }

    std::string toString() const override
    {
        std::ostringstream oss;
        oss << "client socket context info"
            << ", fd: " << mClientFd
            << ", port:" << mPort
            << ", address:" << mClientAddrStr
            ;

        return oss.str();
    }

    int mClientFd;
    int mPort;
    std::string mClientAddrStr;
    std::vector<unsigned char> mRequest;
    std::vector<unsigned char> mResponse;
    std::mutex mMutex;
    std::mutex mRequestMutex;
    std::mutex mResponseMutex;
};
typedef std::shared_ptr<SocketContext> SocketContextPtr;

class IPV4SocketContext : public SocketContext
{
public:
    IPV4SocketContext(int fd, int port, const struct sockaddr_in& addr)
        : SocketContext(fd, port)
        , mClientAddr(addr)
    {}

	virtual ~IPV4SocketContext()
	{}

    struct sockaddr_in mClientAddr;
};
typedef std::shared_ptr<IPV4SocketContext> IPV4SocketContextPtr;

class IPV6SocketContext : public SocketContext
{
public:
    IPV6SocketContext(int fd, int port, const struct sockaddr_in6& addr)
        : SocketContext(fd, port)
        , mClientAddr(addr)
    {}

	virtual ~IPV6SocketContext()
	{}

    struct sockaddr_in6 mClientAddr;
};
typedef std::shared_ptr<IPV6SocketContext> IPV6SocketContextPtr;

class SocketContextQueue : public std::queue<SocketContextPtr>
{
};
typedef std::shared_ptr<SocketContextQueue> SocketContextQueuePtr;

NU_END

#endif
