#ifndef __NET_UTILS_IO_EVENT_LOOP_THREAD_H__
#define __NET_UTILS_IO_EVENT_LOOP_THREAD_H__

NU_BEGIN

#include <boost/thread.hpp>
#include <NetUtils/common.h>
#include <NetUtils/base/SocketBuffer.h>

class IOEventLoopThread
{
public:
    bool run();

    // save the local buffer and try to consume readEvents
    bool onInternelReadNotify(SocketContextSPtr sock_ctx, const char* data, size_t len);

    // save the local buffer and try to consume writeEvents 
    bool onInternelWriteNotify(SocketContextSPtr sock_ctx, const char* data, size_t len);

protected:
    bool canProcessEvent(IOEvent event);
    bool preProcessEvent(IOEvent event);
    bool processEvent(IOEvent event);
    bool postProcessEvent(IOEvent event);

    bool processReadEvent(IOEvent event);
    bool processWriteEvent(IOEvent event);

protected:
    std::vector<SocketBufferSPtr> SocketContexts;
    std::list<IOEvent> mIOEvents;
    std::mutex mMutex;
    std::condition_variable mIOEventsCond;
};
typedef std::shared_ptr<IOEventLoopThread> IOEventLoopThreadSPtr
typedef std::weak_ptr<IOEventLoopThread> IOEventLoopThreadWPtr

NU_END


#endif
