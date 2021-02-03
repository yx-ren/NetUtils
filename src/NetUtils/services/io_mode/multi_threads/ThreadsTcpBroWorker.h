#ifndef __NET_UTILS_SERVICES_THREADS_TCP_BRO_WORKER_H__
#define __NET_UTILS_SERVICES_THREADS_TCP_BRO_WORKER_H__

#include <mutex>
#include <condition_variable>
#include <NetUtils/base/SocketBufferContext.h>
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

#if 1
    void registerInternelReadCompleteCb(InternelReadCompleteCb cb);
    void registerInternelWriteCompleteCb(InternelWriteCompleteCb cb);
#endif

protected:
    virtual void handleReadEvent(IPV4SocketContextSPtr sock_ctx);
    virtual void handleWriteEvent(IPV4SocketContextSPtr sock_ctx);

private:
    bool mIsReadEnabled;
    bool mIsWriteEnabled;
    std::mutex mReadMutex;
    std::condition_variable mReadCond;
    std::mutex mWriteMutex;
    std::condition_variable mWriteCond;
    boost::thread mRecvThread;
    boost::thread mSendThread;
#if 1
    InternelReadCompleteCb mInternelReadCb;
    InternelWriteCompleteCb mInternelWriteCb;
#endif
};
typedef std::shared_ptr<ThreadsTcpBroWorker> ThreadsTcpBroWorkerSPtr;
typedef std::weak_ptr<ThreadsTcpBroWorker> ThreadsTcpBroWorkerWPtr;

NU_SER_END

#endif
