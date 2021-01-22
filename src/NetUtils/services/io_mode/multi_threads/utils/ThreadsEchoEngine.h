#ifndef __NET_UTILS_SERVICES_THREADS_ECHO_ENGINE_H__
#define __NET_UTILS_SERVICES_THREADS_ECHO_ENGINE_H__

NU_SER_BEGIN

#include <mutex>
#include <condition_variable>
#include <boost/thread.hpp>

#include <NetUtils/common.h>
#include <NetUtils/SocketContext.h>
#include <NetUtils/services/common.h>
#include <NetUtils/services/IEngine.h>

class ThreadsEchoEngine : public IEngine
{
public:
    ThreadsEchoEngine(const char *tag)
        : IEngine(tag)
        , mIsRunning(false)
    {}

    ThreadsEchoEngine(const char *tag, int threads)
        : IEngine(tag)
        , mIsRunning(false)
        , mThreads(threads)
    {}

    virtual ~ThreadsEchoEngine() {}

    virtual std::string toString() const override;

    virtual bool start() override;
    virtual void stop() override;
    virtual bool isStarted() const override;

    virtual bool process(IContextSPtr context) override;

protected:
    void process_task();

protected:
    bool mIsRunning;
    int mThreads;
    std::mutex mMutex;
    std::condition_variable mTaskQueCond;
    SocketContextQueue mTaskQueue;
    std::vector<std::shared_ptr<boost::thread>> mWorkThreads;
};

NU_SER_END

#endif
