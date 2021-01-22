#ifndef __NET_UTILS_SERVICES_THREADS_CLIENT_HANDEL_H__
#define __NET_UTILS_SERVICES_THREADS_CLIENT_HANDEL_H__

#include <memory>
#include <string.h>
#include <list>
#include <queue>
#include <thread>
#if defined(WIN32) || defined(_WIN32)
#include <mutex>
#else
#include <condition_variable>
#endif

#if defined(WIN32) || defined (_WIN32)
#include <ws2tcpip.h>
#include <WinSock.h>
#include <process.h>
#include <mstcpip.h>
#else
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

#include <NetUtils/services/io_mode/SocketContext.h>

NU_SER_BEGIN

class ThreadsClientHandle
{
public:
    explicit ThreadsClientHandle(SocketContextPtr context)
        : mContext(context)
    {}

    virtual ~ThreadsClientHandle()
    {}

    virtual void doWork(void) = 0;

protected:
    virtual void handleReadEvent(void) = 0;

    virtual void handleWriteEvent(void) = 0;

    std::string dumpHex(const char* buf, int len);

protected:
    std::mutex mMutex;
    std::condition_variable mCond;
    SocketContextPtr mContext;

    std::queue<std::shared_ptr<std::string>> mMessageQueue;
};
typedef std::shared_ptr<ThreadsClientHandle> ThreadsClientHandlePtr;

NU_SER_END

#endif
