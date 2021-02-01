#ifndef __NET_UTILS_SERVICES_IPV4_THREADS_TCP_SERVER_H__
#define __NET_UTILS_SERVICES_IPV4_THREADS_TCP_SERVER_H__

#include <NetUtils/services/io_mode/TcpServer.h>
#include <NetUtils/services/io_mode/SocketContext.h>
#include <NetUtils/services/io_mode/multi_threads/ThreadsClientHandle.h>

NU_SER_BEGIN

class IPV4ThreadsClientHandle : public ThreadsClientHandle
{
public:
    explicit IPV4ThreadsClientHandle(IPV4SocketContextSPtr context)
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

    virtual void handleClient(SocketContextSPtr context);
};
typedef std::shared_ptr<IPV4ThreadsTcpServer> IPV4ThreadsTcpServerPtr;

NU_SER_END

#endif
