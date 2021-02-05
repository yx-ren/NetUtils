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

    bool process(SocketContextSPtr sock_ctx);

#if 0
    // save the local buffer and try to consume readEvents
    bool onInternelReadNotify(SocketContextSPtr sock_ctx, const char* data, size_t len);

    // save the local buffer and try to consume writeEvents 
    bool onInternelWriteNotify(SocketContextSPtr sock_ctx, const char* data, size_t len);
#endif

    bool addIOEvent(IOEventSPtr event);

    //bool removeIOEvent(IOEventSPtr event);

    size_t getSocketSize() const
    {
        return mSocketContexts.size();
    }

    size_t getEventSize() const
    {
        return mIOEvents.size();
    }

protected:
    bool run();

    bool canProcessEvent(IOEvent event);

    bool preProcessEvent(IOEvent event);
    bool processEvent(IOEvent event);
    bool postProcessEvent(IOEvent event);

    bool processReadEvent(IOEvent event);
    bool processWriteEvent(IOEvent event);

protected:
    std::shared_ptr<boost::thread> mThread;
    std::vector<SocketBufferContextSPtr> mSocketContexts;
    std::list<IOEvent> mIOEvents;
    std::mutex mMutex; // replace rwlock, TODO......
    std::condition_variable mIOEventsCond;
    bool mIsRunning;
    std::weak_ptr<IOEventLoopThreadPool> mOwner;
};
typedef std::shared_ptr<IOEventLoopThread> IOEventLoopThreadSPtr
typedef std::weak_ptr<IOEventLoopThread> IOEventLoopThreadWPtr

NU_END


#endif
