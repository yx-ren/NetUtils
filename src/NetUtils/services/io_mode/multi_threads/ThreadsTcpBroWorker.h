#ifndef __NET_UTILS_SERVICES_THREADS_TCP_BRO_WORKER_H__
#define __NET_UTILS_SERVICES_THREADS_TCP_BRO_WORKER_H__

#include <mutex>
#ifndef WIN32
#include <condition_variable>
#endif
#include <NetUtils/services/IWorker.h>

NU_SER_BEGIN

class ThreadsTcpBroWorker : public IWorker
{
public:
    ThreadsTcpBroWorker()
        : IWorker()
        , mIsReadEnabled(true)
        , mIsWriteEnabled(false)
    {}

    ThreadsTcpBroWorker(IContextSPtr context)
        : IWorker(context)
        , mIsReadEnabled(true)
        , mIsWriteEnabled(false)
    {}

    virtual ~ThreadsTcpBroWorker() {}

    virtual bool doWork() override;

#if 0
    bool isReadEventEnabled();
    bool isWriteEventEnabled();
#endif

    void enableReadEvent();
    void enableWriteEvent();

    void disbaleReadEvent();
    void disbaleWriteEvent();

protected:
    virtual void handleReadEvent();
    virtual void handleWriteEvent();

private:
    bool mIsReadEnabled;
    bool mIsWriteEnabled;
    std::mutex mReadMutex;
    std::condition_variable mReadCond;
    std::mutex mWriteMutex;
    std::condition_variable mWriteCond;
    boost::thread mRecvThread;
    boost::thread mSendThread;
};
typedef std::shared_ptr<ThreadsTcpBroWorker> ThreadsTcpBroWorkerSPtr;
typedef std::weak_ptr<ThreadsTcpBroWorker> ThreadsTcpBroWorkerWPtr;

NU_SER_END

#endif
