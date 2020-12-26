#include <NetUtils/services/tcp/multi_threads/ThreadsTcpBroProcessor.h>
#include <memory>
#include <thread>
#include <NetUtils/services/tcp/multi_threads/ThreadsTcpBroWorker.h>

NU_SER_BEGIN


bool ThreadsTcpBroProcessor::process(IContextSPtr context)
{
    auto process_client = [](IContextSPtr context)
    {
        ThreadsTcpBroWorkerSPtr worker = std::make_shared<ThreadsTcpBroWorker>(context);
        worker->doWork();
    };

    std::thread worker_thread = std::thread(std::bind(process_client, context));
    worker_thread.detach();

    return true;
}

NU_SER_END
