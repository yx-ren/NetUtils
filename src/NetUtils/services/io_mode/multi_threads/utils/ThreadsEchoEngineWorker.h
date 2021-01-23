#ifndef __NET_UTILS_SERVICES_THREADS_ECHO_ENGINE_WORKER_H__
#define __NET_UTILS_SERVICES_THREADS_ECHO_ENGINE_WORKER_H__

#include <NetUtils/services/IWorker.h>

NU_SER_BEGIN

class ThreadsEchoEngineWorker : public IWorker
{
public:
    ThreadsTcpBroWorker()
        : IWorker()
        , mRunning(true)
        , mIsReadEnabled(true)
        , mIsWriteEnabled(false)
    {}

    ThreadsTcpBroWorker(IContextSPtr context)
        : IWorker(context)
        , mRunning(true)
        , mIsReadEnabled(true)
        , mIsWriteEnabled(false)
    {}

    virtual ~ThreadsTcpBroWorker() {}

    virtual bool doWork() override;

};

NU_SER_END

#endif
