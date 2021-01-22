#include <NetUtils/services/io_mode/multi_threads/utils/ThreadsEchoEngine.h>
#include <assert>
#include <functional>
#include <map>
#include <boost/make_shared.hpp>
#include <common/base/logger/LoggerManager.h>

using namespace CBASE_LOGGER_NAMESPACE;

NU_SER_BEGIN

std::string ThreadsEchoEngine::toString() const
{
    return "TODO......";
}

bool ThreadsEchoEngine::start()
{
    assert (mThreads > 0);

    std::unique_lock<std::mutex> lk(mMutex);

    for (int i = 0; i != mThreads; i++)
    {
        mWorkThreads.push_back(
            boost::make_shared<boost::thread>(
                boost::bind(&ThreadsEchoEngine::process_task, this)));
    }

    return true;
}

void ThreadsEchoEngine::stop()
{
    std::unique_lock<std::mutex> lk(mMutex);

    for (auto& thread : mWorkThreads)
    {
        thread->interrupt();
        thread->join();
    }

    mIsRunning = false;
}

bool ThreadsEchoEngine::isStarted() const
{
    return mIsRunning;
}

bool ThreadsEchoEngine::process(IContextSPtr context)
{
    return true;
}

NU_SER_END
