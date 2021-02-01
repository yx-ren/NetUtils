#include <NetUtils/services/io_mode/multi_threads/utils/ThreadsEchoEngineWorker.h>

#include <common/base/logger/Logger.h>
#include <common/base/logger/LoggerManager.h>

#include <NetUtils/IOHelper.h>
#include <NetUtils/SocketContext.h>

NU_SER_BEGIN

bool ThreadsEchoEngineWorker::doWork()
{
    CBT_DEBUG("ThreadsEchoEngineWorker", "doWork() worker was running");

    // TODO......
    IPV4SocketContextSPtr socket_context = std::dynamic_pointer_cast<IPV4SocketContext>(mContext);
    if (socket_context == NULL)
    {
        CBT_WARN("ThreadsEchoEngineWorker()", "doWork() context cast failed(SocketContextSPtr -> IPV4SocketContextSPtr)"
                << ", skip this task");
        return;
    }

    CBT_DEBUG("ThreadsEchoEngineWorker", "doWork() worker was done");

    return true;
}

NU_SER_END
