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

    bool isRunning();

    bool process(SocketContextSPtr sock_ctx);

    // save the local buffer and try to consume readEvents
    bool onInternelReadNotify(SocketContextSPtr sock_ctx, const char* data, size_t len);

    // save the local buffer and try to consume writeEvents 
    bool onInternelWriteNotify(SocketContextSPtr sock_ctx, const char* data, size_t len);

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
    std::vector<SocketBufferContextSPtr> SocketContexts;
    std::list<IOEvent> mIOEvents;
    std::mutex mMutex;
    std::condition_variable mIOEventsCond;
    bool mIsRunning;
};
typedef std::shared_ptr<IOEventLoopThread> IOEventLoopThreadSPtr
typedef std::weak_ptr<IOEventLoopThread> IOEventLoopThreadWPtr

NU_END


#endif
