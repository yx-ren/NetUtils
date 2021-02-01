#include <NetUtils/IOEventLoopThreadPool.h>

NU_BEGIN

bool IOEventLoopThreadPool::start()
{
    for (int i = 0; i != mThreads; i++)
    {
        IOEventLoopThreadSPtr thread = std::make_shared<IOEventLoopThread>();
        if (!thread->start())
            return false;

        mThreads.push_back(thread);
    }

    return true;
}

bool IOEventLoopThreadPool::stop()
{
    for (auto th : mThreads)
        th->stop();

    return true;
}

bool IOEventLoopThreadPool::process(SocketContextSPtr sock_ctx)
{
    return true;
}

NU_END
