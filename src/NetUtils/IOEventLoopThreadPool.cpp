#include <NetUtils/IOEventLoopThreadPool.h>

NU_BEGIN

bool IOEventLoopThreadPool::start()
{
    std::unique_lock<std::mutex> lk(mLock);
    for (int i = 0; i != mMaxThreadNum; i++)
    {
        IOEventLoopThreadSPtr thread = std::make_shared<IOEventLoopThread>();
        if (!thread->start())
            return false;

        thread->setOwner(std::enable_shared_from_this());

        mThreads.push_back(thread);
    }

    return true;
}

bool IOEventLoopThreadPool::stop()
{
    std::unique_lock<std::mutex> lk(mLock);
    for (auto th : mThreads)
        th->stop();

    return true;
}

bool IOEventLoopThreadPool::process(SocketContextSPtr sock_ctx)
{
    std::unique_lock<std::mutex> lk(mLock);
    auto min_load = std::min_element(mThreads.begin(), mThreads.end(),
            [](const IOEventLoopThreadSPtr lhs, const IOEventLoopThreadSPtr rhs)
            {
                return lhs->getSocketSize() < lhs->getSocketSize();
            });

    min_load->process(sock_ctx);

    return true;
}

NU_END
