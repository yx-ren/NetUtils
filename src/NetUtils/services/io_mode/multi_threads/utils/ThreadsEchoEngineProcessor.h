#ifndef __NET_UTILS_SERVICES_THREADS_ECHO_ENGINE_PROCESSOR_H__
#define __NET_UTILS_SERVICES_THREADS_ECHO_ENGINE_PROCESSOR_H__

NU_SER_BEGIN

class ThreadsEchoEngineProcessor : public IProcessor
{
public:
    virtual ~ThreadsEchoEngineProcessor() {}

    virtual bool process(IContextSPtr context) override;
};
typedef std::shared_ptr<ThreadsEchoEngineProcessor> ThreadsEchoEngineProcessorSPtr;
typedef std::weak_ptr<ThreadsEchoEngineProcessor> ThreadsEchoEngineProcessorWPtr;

NU_SER_END

#endif
