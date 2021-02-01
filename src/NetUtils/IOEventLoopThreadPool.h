#ifndef __NET_UTILS_IO_EVENT_LOOP_THREAD_H__
#define __NET_UTILS_IO_EVENT_LOOP_THREAD_H__

NU_BEGIN

#include <boost/thread.hpp>
#include <NetUtils/common.h>
#include <NetUtils/IOEventLoopThread.h>

class IOEventLoopThreadPool
{
public:
    IOEventLoopThreadPool(int maxThreadNum = 4)
        : mMaxThreadNum(maxThreadNum)
    {}

    bool start();

    bool stop();

    bool process(SocketContextSPtr sock_ctx);

private:
    int mMaxThreadNum;
    std::vector<IOEventLoopThreadSPtr> mThreads;
};


NU_END
