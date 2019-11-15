#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__

#define INVALID_FD              -1
#define BACK_LOG                0xff
#define MAX_MESSAGEBUFF_LEN     0xffff
#if (defined WIN32) || (defined _WIN32)
#define GET_LAST_SOCKET_ERROR WSAGetLastError()
#else
#define GET_LAST_SOCKET_ERROR errno
#endif

#include <string>
#include <memory>


class TcpServer
{
public:
    explicit TcpServer(uint16_t port)
        : mIp(""), mPort(port), mFd(INVALID_FD)
    {}

    explicit TcpServer(const std::string ip, uint16_t port)
        : mIp(ip), mPort(port), mFd(INVALID_FD)
    {}

    virtual ~TcpServer()
    {}

    virtual bool start(void);

    virtual bool stop(void);

protected:
    TcpServer(const TcpServer&) = delete;
    TcpServer& operator=(const TcpServer&) = delete;

protected:
    virtual bool initSocket(void) = 0;

    virtual bool run(void) = 0;

protected:
    std::string mIp;
    uint16_t mPort;
    int32_t mFd;
};
typedef std::shared_ptr<TcpServer> TcpServerPtr;

#endif
