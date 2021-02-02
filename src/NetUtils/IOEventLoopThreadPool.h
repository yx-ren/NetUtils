#ifndef __NET_UTILS_IO_EVENT_LOOP_THREAD_H__
#define __NET_UTILS_IO_EVENT_LOOP_THREAD_H__

NU_BEGIN

#include <boost/thread.hpp>
#include <NetUtils/common.h>
#include <NetUtils/IOEventLoopThread.h>

class IOEventLoopThreadPool : public SingleObject
{
public:
    virtual ~IOEventLoopThreadPool() {}

    void setMaxThreadNum(int num) { mMaxThreadNum = num; }
    int getMaxThreadNum() const { return mMaxThreadNum; }

    bool start();

    bool stop();

    bool process(SocketContextSPtr sock_ctx);

protected:
    IOEventLoopThreadPool()
        : mMaxThreadNum(4)
    {}

private:
    int mMaxThreadNum;
    std::mutex mLock;
    std::vector<IOEventLoopThreadSPtr> mThreads;
};


NU_END
