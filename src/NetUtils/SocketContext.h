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
#include <NetUtils/base/Buffer.h>

NU_BEGIN

#define INVALID_FD              -1

enum ASYNC_IO_RESULT
{
    AIR_READ_SUCCESSED,
    AIR_WRITE_SUCCESSED,
    AIR_READ_FAILED,
    AIR_WRITE_FAILED,
    AIR_PEER_CLOSED,
};

typedef std::function<void(ASYNC_IO_RESULT result, const char* data, size_t size)> ExternelIOCompleteCb;
typedef std::function<void(ASYNC_IO_RESULT result, const char* data, size_t size)> ExternelReadCompleteCb;
typedef std::function<void(ASYNC_IO_RESULT result, const char* data, size_t size)> ExternelWriteCompleteCb;

#if 0
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

    void registerExternelReadCompleteCb(ExternelReadCompleteCb read_cb)
    {
        mExternelReadCompleteCb = read_cb;
    }

    void registerExternelWriteCompleteCb(ExternelWriteCompleteCb write_cb)
    {
        mExternelWriteCompleteCb = write_cb;
    }

    std::string toString() const override
    {
        std::ostringstream oss;
        oss << "fd: " << mClientFd
            << ", port:" << mPort
            << ", address:" << mClientAddrStr
            ;

        return oss.str();
    }

    bool read(char* data, size_t len);
    bool write(const char* data, size_t len);

    bool read_async(size_t len);
    bool write_async(const char* data, size_t len);

    int mClientFd;
    int mPort;
    std::string mClientAddrStr;
    std::mutex mMutex;
#ifdef __BUFF_OBJECT__
    // TODO......
    Buffer mReadBuffer;
    Buffer mWriteBuffer;
#else
    std::vector<unsigned char> mRequest;
    std::vector<unsigned char> mResponse;
    std::mutex mRequestMutex;
    std::mutex mResponseMutex;
#endif

    ExternelReadCompleteCb mExternelReadCompleteCb; // called by consume per event
    ExternelWriteCompleteCb mExternelWriteCompleteCb; // called by consume per event
};
#else
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
        oss << "fd: " << mClientFd
            << ", port:" << mPort
            << ", address:" << mClientAddrStr
            ;

        return oss.str();
    }

    int mClientFd;
    int mPort;
    std::string mClientAddrStr;
    std::mutex mMutex;
};
#endif
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
