#include <NetUtils/IOEventLoopThread.h>

NU_BEGIN

bool IOEventLoopThread::start()
{
    mThread = std::make_shared<boost::thread>(std::bind(&IOEventLoopThread::run, this));

    // sleep a while make sure that the IOEventLoopThread::run has been exec
    // if not, the thread object(IOEventLoopThread) may be destrouct before the new thread(run()) run
    // this may be cause an error
    while(!isRunning())
        std::this_thread::sleep_for(std::chrono::milliseconds(20));

    return true;
}

bool IOEventLoopThread::stop()
{
    mThread->interrupt();
    mThread->join();

    return true;
}

bool IOEventLoopThread::isRunning()
{
    std::unique_lock<std::mutex> lk(mMutex);
    return mIsRunning;
}

void IOEventLoopThread::setOwner(std::weak_ptr<IOEventLoopThreadPool> owner)
{
    mOwner = owner;
}

bool IOEventLoopThread::run()
{
    {
        std::unique_lock<std::mutex> lk(mMutex);
        mIsRunning = true;
    }

    while (true)
    {
        std::unique_lock<std::mutex> lk(mMutex);
        mIOEventsCond.wait(lk, !mIOEvents.empty());
        boost::interruption_point();

        // try to consume a event
        event = mIOEvents.front(); // this may be one can't consumed event block the whole event loop, try to fix it
        if (canProcessEvent(event))
        {
            preProcessEvent(event);

            if (processEvent(event))
            {
                // log, TODO......
            }
            else
            {
                // log, TODO......
            }

            postProcessEvent(event);

            mIOEvents.pop();
        }
        else
        {
            continue;
        }
    }
    return true;
}

bool IOEventLoopThread::process(SocketContextSPtr sock_ctx)
{
    std::unique_lock<std::mutex> lk(mMutex);
    mSocketContexts.push_back(sock_ctx);
    return true;
}

#if 0
bool IOEventLoopThread::onInternelReadNotify(SocketContextSPtr sock_ctx, const char* data, size_t len)
{
    const auto sock_buffer = std::find_if(mSocketContexts.begin(), mSocketContexts.end(),
            [](const SocketBufferContextSPtr buff) { buff->mSockContext == sock_ctx; });

    if (sock_buffer == mSocketContexts.end())
    {
        CBT_WARN("IOEventLoopThread", "onInternelReadNotify() socket:" << sock_ctx->toString() << " not found in this event loop");
        return true;
    }

    // save the read data from kernel
    // TODO......
    sock_buffer->storeKernelRB(data, len);

    // notify the read event and try to consume a readEvents or not
    // TODO......
    mIOEventsCond.notify()
}

bool IOEventLoopThread::onInternelWriteNotify(SocketContextSPtr sock_ctx, const char* data, size_t len)
{
    // TODO......
}
#endif

// can process event: the socket buffer has enough data to consume a read/wirte event
bool IOEventLoopThread::canProcessEvent(IOEvent event)
{
    return true;
}

bool IOEventLoopThread::preProcessEvent(IOEvent event)
{
    return true;
}

bool IOEventLoopThread::processEvent(IOEvent event)
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

    return process_successed;
}

bool IOEventLoopThread::postProcessEvent(IOEvent event)
{
    return true;
}

bool IOEventLoopThread::processReadEvent(IOEvent event)
{
    //mExternelReadCompleteCb();
    return true;
}

bool IOEventLoopThread::processWriteEvent(IOEvent event)
{
    //mExternelWriteCompleteCb();
    return true;
}

NU_END
