#ifndef __NET_UTILS_SERVICES_MULTI_THREADS_TCP_BRO_SERVER_H__
#define __NET_UTILS_SERVICES_MULTI_THREADS_TCP_BRO_SERVER_H__

#if 0
#include <NetUtils/services/io_mode/TcpServer.h>
#include <NetUtils/services/io_mode/SocketContext.h>
#include <NetUtils/services/io_mode/multi_threads/ThreadsClientHandle.h>
#endif

NU_SER_BEGIN

class MultiThreadsTcpBroServer : public IBrokerServerEngine
{
public:
    explicit MultiThreadsTcpBroServer(protocol::PROTOCOL_TYPE type)
        : IBrokerServerEngine(type)
    {}

    explicit MultiThreadsTcpBroServer(protocol::PROTOCOL_TYPE type,const std::string ip, uint16_t port)
        : IBrokerServerEngine(type, ip, port)
    {}

    virtual ~MultiThreadsTcpBroServer()
    {}

protected:
    virtual bool run(void) override;

    //virtual void handleClient(SocketContextPtr context);

protected:
    IProcessorSptr mClientProcessor;
};
typedef std::shared_ptr<MultiThreadsTcpBroServer> MultiThreadsTcpBroServerSPtr;

NU_SER_END

#endif
