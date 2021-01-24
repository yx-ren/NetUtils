#include <NetUtils/services/io_mode/multi_threads/utils/ThreadsEchoEngineProcessor.h>
#include <NetUtils/services/io_mode/multi_threads/utils/ThreadsEchoEngineWorker.h>

NU_SER_BEGIN

bool ThreadsEchoEngineProcessor::process(IContextSPtr context)
{
    ThreadsEchoEngineWorkerSPtr worker(std::make_shared<ThreadsEchoEngineWorker>(context));

    worker->doWork();

    return true;
}

NU_SER_END
