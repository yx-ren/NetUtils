#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__

#define INVALID_FD              -1
#define BACK_LOG                0xff
#define MAX_MESSAGEBUFF_LEN     1024 * 1024 * 1
#if (defined WIN32) || (defined _WIN32)
#define GET_LAST_SOCKET_ERROR WSAGetLastError()
#else
#define GET_LAST_SOCKET_ERROR errno
#endif

#if (defined WIN32) || (defined _WIN32)
#include <WinSock2.h>
#else
#endif

#if (defined WIN32) || (defined _WIN32)
#define CLOSE_SOCKET(fd) closesocket(fd)
#else
#define GET_LAST_SOCKET_ERROR errno
#define CLOSE_SOCKET(fd) close(fd)
#endif

#include <string>
#include <memory>
#include <vector>


class TcpServer
{
public:
    typedef std::shared_ptr<std::function<bool(const int sock)>> SetSockOptCBPtr;
    typedef std::shared_ptr<std::vector<SetSockOptCBPtr>> SetSockOptCBsVectorPtr;

    explicit TcpServer(uint16_t port);

    explicit TcpServer(const std::string ip, uint16_t port);

    virtual ~TcpServer();

    virtual bool start(void);

    virtual bool stop(void);

    virtual void setListenSockCBs(SetSockOptCBsVectorPtr listenSockCBs);

    virtual void setAcceptSockCBs(SetSockOptCBsVectorPtr acceptSockCBs);

protected:
    TcpServer(const TcpServer&) = delete;
    TcpServer& operator=(const TcpServer&) = delete;

protected:
    virtual bool initSocket(void) = 0;

    virtual bool run(void) = 0;

    virtual void applyListenSockCBs(int listenSock);

    virtual void applyAcceptSockCBs(int acceptSock);

protected:
    SetSockOptCBsVectorPtr mListenSockCBs;
    SetSockOptCBsVectorPtr mAcceptSockCBs;
    std::string mIp;
    uint16_t mPort;
    int32_t mFd;
};
typedef std::shared_ptr<TcpServer> TcpServerPtr;

#endif
