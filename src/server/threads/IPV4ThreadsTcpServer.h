#ifndef __IPV4_THREADS_TCP_SERVER_H__
#define __IPV4_THREADS_TCP_SERVER_H__

#include "ThreadsClientHandle.h"

class IPV4SocketContext : public SocketContext
{
public:
    IPV4SocketContext(const struct sockaddr_in& addr, int fd)
        : SocketContext(fd)
        , mClientAddr(addr)
    {}

    struct sockaddr_in mClientAddr;
};
typedef std::shared_ptr<IPV4SocketContext> IPV4SocketContextPtr;

class IPV4ThreadsClientHandle : public ThreadsClientHandle
{
public:
    explicit IPV4ThreadsClientHandle(IPV4SocketContextPtr context)
        : ThreadsClientHandle(context)
    {}

    virtual ~IPV4ThreadsClientHandle()
    {}

    virtual void doWork(void) override;

protected:
    virtual void handleReadEvent(void) override;

    virtual void handleWriteEvent(void) override;
};
typedef std::shared_ptr<IPV4ThreadsClientHandle> IPV4ThreadsClientHandleHandlePtr;

class IPV4ThreadsTcpServer : public TcpServer
{
public:
    explicit IPV4ThreadsTcpServer(uint16_t port)
        : TcpServer(port)
    {}

    explicit IPV4ThreadsTcpServer(const std::string ip, uint16_t port)
        : TcpServer(ip, port)
    {}

    virtual ~IPV4ThreadsTcpServer()
    {}

protected:
    virtual bool initSocket(void) override;

    virtual bool run(void) override;

    virtual void handleClient(SocketContextPtr context);
};
typedef std::shared_ptr<IPV4ThreadsTcpServer> IPV4ThreadsTcpServerPtr;

#endif
