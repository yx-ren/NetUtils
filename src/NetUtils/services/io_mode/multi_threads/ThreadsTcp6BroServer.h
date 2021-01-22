#ifndef __NET_UTILS_SERVICES_THREADS_TCP6_BRO_SERVER_H__
#define __NET_UTILS_SERVICES_THREADS_TCP6_BRO_SERVER_H__

#include <NetUtils/services/io_mode/ITcpBrokerServerEngine.h>
#include <NetUtils/services/io_mode/multi_threads/ThreadsTcpBroProcessor.h>

NU_SER_BEGIN

class ThreadsTcp6BroServer : public ITcpBrokerServerEngine
{
public:
    ThreadsTcp6BroServer(const char* tag)
        : ITcpBrokerServerEngine(tag, protocol::PROTOCOL_TYPE::PT_TCP_IPV6)
        , mClientProcessor(std::make_shared<ThreadsTcpBroProcessor>())
    {}

    ThreadsTcp6BroServer(const char* tag, const std::string ip, uint16_t port)
        : ITcpBrokerServerEngine(tag, protocol::PROTOCOL_TYPE::PT_TCP_IPV6, ip, port)
        , mClientProcessor(std::make_shared<ThreadsTcpBroProcessor>())
    {}

    virtual ~ThreadsTcp6BroServer()
    {}

    virtual bool process(IContextSPtr context) override;

protected:
    virtual bool run() override;

protected:
    ThreadsTcpBroProcessorSPtr mClientProcessor;
};
typedef std::shared_ptr<ThreadsTcp6BroServer> ThreadsTcp6BroServerSPtr;

NU_SER_END

#endif
