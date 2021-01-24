#ifndef __NET_UTILS_SERVICES_THREADS_ECHO_ENGINE_WORKER_H__
#define __NET_UTILS_SERVICES_THREADS_ECHO_ENGINE_WORKER_H__

#include <NetUtils/services/IWorker.h>

NU_SER_BEGIN

class ThreadsEchoEngineWorker : public IWorker
{
public:
    ThreadsTcpBroWorker()
        : IWorker()
    {}

    ThreadsTcpBroWorker(IContextSPtr context)
        : IWorker(context)
    {}

    virtual ~ThreadsTcpBroWorker() {}

    virtual bool doWork() override;
};
typedef std::shared_ptr<ThreadsEchoEngineWorker> ThreadsEchoEngineWorkerSPtr;
typedef std::weak_ptr<ThreadsEchoEngineWorker> ThreadsEchoEngineWorkerWPtr;

NU_SER_END

#endif
