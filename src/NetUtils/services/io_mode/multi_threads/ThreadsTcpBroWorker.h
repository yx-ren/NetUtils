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

    bool isReadEventEnabled() const;
    bool isWriteEventEnabled() const;

    void enableReadEvent();
    void enableWriteEvent();

    void disbaleReadEvent();
    void disbaleWriteEvent();

    // consider call event loop api directly, because if register callback
    // the event loop thread may be exited when callback triggerd
    // may be not do this, because code has checked before use
    // TODO......
    void registerInternelReadCompleteCb(InternelReadCompleteCb cb);
    void registerInternelWriteCompleteCb(InternelWriteCompleteCb cb);

protected:
    virtual void handleReadEvent(SocketBufferContextSPtr sock_buf_ctx, IPV4SocketContextSPtr sock_ctx);
    virtual void handleWriteEvent(SocketBufferContextSPtr sock_buf_ctx, IPV4SocketContextSPtr sock_ctx);

private:
    bool mIsReadEnabled;
    bool mIsWriteEnabled;
    std::mutex mMutex;
    std::mutex mReadMutex;
    std::condition_variable mReadCond;
    std::mutex mWriteMutex;
    std::condition_variable mWriteCond;
    boost::thread mRecvThread;
    boost::thread mSendThread;
    InternelReadCompleteCb mInternelReadCb;
    InternelWriteCompleteCb mInternelWriteCb;
};
typedef std::shared_ptr<ThreadsTcpBroWorker> ThreadsTcpBroWorkerSPtr;
typedef std::weak_ptr<ThreadsTcpBroWorker> ThreadsTcpBroWorkerWPtr;

NU_SER_END

#endif
