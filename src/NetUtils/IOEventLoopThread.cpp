#include <NetUtils/IOEventLoopThread.h>

NU_BEGIN

bool IOEventLoopThread::run()
{
    while (true)
    {
        std::unique_lock<std::mutex> lk(mMutex);
        mIOEventsCond.wait(lk, !mIOEvents.empty());

        // try to consume a event
        event = mIOEvents.front();
        if (canProcessEvent(event))
        {
#if 0
            processEvent(event);
            |
                // readBuffer reset pos
                // TODO......

                // notify caller by async read
                // TODO......
                mExternelReadCompleteCb();

            if (processed)
                mIOEvents.pop();
#else
            if (processEvent(event))
            {
                // log, TODO......
            }
            else
            {
                // log, TODO......
            }

            mIOEvents.pop();
#endif
        }
        else
        {
            continue;
        }
    }
    return true;
}

bool IOEventLoopThread::onInternelReadNotify(SocketContextSPtr sock_ctx, const char* data, size_t len)
{
    const auto sock_buffer = std::find_if(SocketContexts.begin(), SocketContexts.end(),
            [](const SocketBufferSPtr buff) { buff->mSockContext == sock_ctx; });

    if (sock_buffer == SocketContexts.end())
    {
        CBT_WARN("IOEventLoopThread", "onInternelReadNotify() socket:" << sock_ctx->toString() << " not found in this event loop");
        return true;
    }

    // save the read data from kernel
    // TODO......
    sock_buffer->storeKernelRB(data, len);

    // notify the read event and try to consume a readEvents or not
    // TODO......
    mIOCond.notify()
}

bool IOEventLoopThread::onInternelWriteNotify(SocketContextSPtr sock_ctx, const char* data, size_t len)
{
    // TODO......
}

bool IOEventLoopThread::canProcessEvent(IOEvent event)
{
    return true;
}

bool IOEventLoopThread::processReadEvent(IOEvent event)
{
    return true;
}

bool IOEventLoopThread::processWriteEvent(IOEvent event)
{
    return true;
}

bool IOEventLoopThread::preProcessEvent(IOEvent event)
{
    bool process_successed = false;
    switch(event->type)
    {
        case read:
            process_successed = processReadEvent(event);
            break;
        case write:
            process_successed = processWriteEvent(event);
            break;
    }

    return true;
}

bool IOEventLoopThread::processEvent(IOEvent event)
{
    return true;
}

bool IOEventLoopThread::postProcessEvent(IOEvent event)
{
    return true;
}

bool IOEventLoopThread::processReadEvent()
{
    return true;
}

bool IOEventLoopThread::processWriteEvent()
{
    return true;
}

NU_END
