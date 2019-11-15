#ifndef __IPV6_THREADS_TCP_SERVER_H__
#define __IPV6_THREADS_TCP_SERVER_H__

#include <server/TcpServer.h>
#include <server/SocketContext.h>
#include "ThreadsClientHandle.h"

class IPV6ThreadsClientHandle : public ThreadsClientHandle
{
public:
    explicit IPV6ThreadsClientHandle(IPV6SocketContextPtr context)
        : ThreadsClientHandle(context)
    {}

    virtual ~IPV6ThreadsClientHandle()
    {}

    virtual void doWork(void) override;

protected:
    virtual void handleReadEvent(void) override;

    virtual void handleWriteEvent(void) override;
};
typedef std::shared_ptr<IPV6ThreadsClientHandle> IPV6ThreadsClientHandleHandlePtr;

class IPV6ThreadsTcpServer : public TcpServer
{
public:
    explicit IPV6ThreadsTcpServer(uint16_t port)
        : TcpServer(port)
    {}

    explicit IPV6ThreadsTcpServer(const std::string ip, uint16_t port)
        : TcpServer(ip, port)
    {}

    virtual ~IPV6ThreadsTcpServer()
    {}

protected:
    virtual bool initSocket(void) override;

    virtual bool run(void) override;

    virtual void handleClient(SocketContextPtr context);
};
typedef std::shared_ptr<IPV6ThreadsTcpServer> IPV6ThreadsTcpServerPtr;

#endif
