#ifndef __NET_UTILS_IO_EVENT_LOOP_THREAD_H__
#define __NET_UTILS_IO_EVENT_LOOP_THREAD_H__

NU_BEGIN

#include <boost/thread.hpp>
#include <NetUtils/common.h>
#include <NetUtils/base/SocketBufferContext.h>

class IOEventLoopThreadPool;

/*
    this class run as a thread
    add io event and async callback when io event was completed
*/
class IOEventLoopThread
{
public:
    IOEventLoopThread()
        : mIsRunning(false)
    {}

    bool start();

    bool stop();

    bool isRunning();

    void setOwner(std::weak_ptr<IOEventLoopThreadPool> owner);
    std::weak_ptr<IOEventLoopThreadPool> getOwner();

    bool process(SocketBufferContextSPtr sock_ctx);

    // save the local buffer and try to consume readEvents
    bool onInternelReadNotify(SocketBufferContextSPtr sock_ctx, const char* data, size_t len);

    // save the local buffer and try to consume writeEvents 
    bool onInternelWriteNotify(SocketBufferContextSPtr sock_ctx, const char* data, size_t len);

    size_t getSocketSize() const
    {
        return mSockBufCtxs.size();
    }

    size_t getEventSize() const
    {
        return mReadyEvents.size();
    }

protected:
    bool run();

protected:
    std::shared_ptr<boost::thread> mThread;
    std::vector<SocketBufferContextSPtr> mSockBufCtxs;
    std::queue<SocketBufferContextSPtr> mReadySockBufCtxs;
    std::mutex mMutex; // replace rwlock, TODO......
    std::condition_variable mReadyCond;
    bool mIsRunning;
    std::weak_ptr<IOEventLoopThreadPool> mOwner;
};
typedef std::shared_ptr<IOEventLoopThread> IOEventLoopThreadSPtr
typedef std::weak_ptr<IOEventLoopThread> IOEventLoopThreadWPtr

NU_END


#endif
