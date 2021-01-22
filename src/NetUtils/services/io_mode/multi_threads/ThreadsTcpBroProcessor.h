#ifndef __NET_UTILS_SERVICES_THREADS_TCP_BRO_PROCESSOR_H__
#define __NET_UTILS_SERVICES_THREADS_TCP_BRO_PROCESSOR_H__

#include <NetUtils/services/IProcessor.h>

NU_SER_BEGIN

class ThreadsTcpBroProcessor : public IProcessor
{
public:
    virtual ~ThreadsTcpBroProcessor() {}

    virtual bool process(IContextSPtr context) override;
};
typedef std::shared_ptr<ThreadsTcpBroProcessor> ThreadsTcpBroProcessorSPtr;
typedef std::weak_ptr<ThreadsTcpBroProcessor> ThreadsTcpBroProcessorWPtr;

NU_SER_END

#endif
