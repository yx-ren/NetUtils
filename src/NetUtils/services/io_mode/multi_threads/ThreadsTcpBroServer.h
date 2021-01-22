#ifndef __NET_UTILS_SERVICES_THREADS_TCP_BRO_SERVER_H__
#define __NET_UTILS_SERVICES_THREADS_TCP_BRO_SERVER_H__

#if 0
#include <NetUtils/services/io_mode/TcpServer.h>
#include <NetUtils/services/io_mode/SocketContext.h>
#include <NetUtils/services/io_mode/multi_threads/ThreadsClientHandle.h>
#else
#include <NetUtils/services/io_mode/ITcpBrokerServerEngine.h>
#include <NetUtils/services/io_mode/multi_threads/ThreadsTcpBroProcessor.h>
#endif

NU_SER_BEGIN

class ThreadsTcpBroServer : public ITcpBrokerServerEngine
{
public:
    ThreadsTcpBroServer(const char* tag)
        : ITcpBrokerServerEngine(tag, protocol::PROTOCOL_TYPE::PT_TCP_IPV4)
        , mClientProcessor(std::make_shared<ThreadsTcpBroProcessor>())
    {}

    ThreadsTcpBroServer(const char* tag, const std::string ip, uint16_t port)
        : ITcpBrokerServerEngine(tag, protocol::PROTOCOL_TYPE::PT_TCP_IPV4, ip, port)
        , mClientProcessor(std::make_shared<ThreadsTcpBroProcessor>())
    {}

    virtual ~ThreadsTcpBroServer()
    {}

    virtual bool process(IContextSPtr context) override;

protected:
    virtual bool run() override;

protected:
    ThreadsTcpBroProcessorSPtr mClientProcessor;
};
typedef std::shared_ptr<ThreadsTcpBroServer> ThreadsTcpBroServerSPtr;

NU_SER_END

#endif
