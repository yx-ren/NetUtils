#include <NetUtils/IOEventLoopThread.h>

NU_BEGIN

bool IOEventLoopThread::start()
{
    if (isRunning())
    {
        CBT_INFO("IOEventLoopThread", "start() thread:0x" << mThread.get_id() << " has been started");
        return true;
    }

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
    if (!isRunning())
    {
        CBT_INFO("IOEventLoopThread", "start() thread has not been started");
        return true;
    }

    mThread->interrupt();
    mThread->join();
    mIsRunning = false;

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

std::weak_ptr<IOEventLoopThreadPool> IOEventLoopThread::getOwner()
{
    return mOwner;
}

bool IOEventLoopThread::run()
{
    {
        std::unique_lock<std::mutex> lk(mMutex);
        mIsRunning = true;
    }

    while (true)
    {
        boost::interruption_point();
        SocketBufferContextSPtr ready_ctx;
        {
            std::unique_lock<std::mutex> lk(mMutex);
            mReadyEventsCond.wait(lk, !mReadySockBufCtxs.empty());
            ready_ctx = mReadySockBufCtxs.front();
            mReadySockBufCtxs.pop();
        }
        boost::interruption_point();

        ready_ctx->processEvent();
    }

    return true;
}

bool IOEventLoopThread::process(SocketBufferContextSPtr sock_ctx)
{
    std::unique_lock<std::mutex> lk(mMutex);
    mSockBufCtxs.push_back(sock_ctx);
    sock_ctx->setOwner(std::enable_shared_from_this);
    return true;
}

bool IOEventLoopThread::onInternelReadNotify(SocketBufferContextSPtr sock_ctx, const char* data, size_t len)
{
    std::unique_lock<std::mutex> lk(mMutex);
    const auto sock_buffer = std::find_if(mSockBufCtxs.begin(), mSockBufCtxs.end(),
            [](const SocketBufferContextSPtr buff) { buff == sock_ctx; });

    if (sock_buffer == mSockBufCtxs.end())
    {
        CBT_WARN("IOEventLoopThread", "onInternelReadNotify() socket:" << sock_ctx->toString() << " not bind in this event loop");
        return true;
    }

    // save the read data from kernel
    sock_buffer->storeKernelRB(data, len);

    // check has ready event or not
    if (!sock_buffer->hasReadyReadEvent())
    {
        CBT_DEBUG("IOEventLoopThread", "onInternelReadNotify() socket:" << sock_ctx->toString() << " not found ready read event");
        return true;
    }

    mReadySockBufCtxs.push(sock_buffer);

    // enable the io event thread, parallel to consume the ready io events
    mReadyEventsCond.notify()
}

bool IOEventLoopThread::onInternelWriteNotify(SocketBufferContextSPtr sock_ctx, const char* data, size_t len)
{
    // TODO......
}

NU_END
