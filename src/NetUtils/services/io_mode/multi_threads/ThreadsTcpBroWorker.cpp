#include <NetUtils/services/io_mode/multi_threads/ThreadsTcpBroWorker.h>
#include <memory>
#include <mutex>
#include <boost/thread.hpp>

#include <common/base/ScopedGuard.h>
#include <common/base/logger/Logger.h>
#include <common/base/logger/LoggerManager.h>

#include <NetUtils/IOHelper.h>

using namespace CBASE_LOGGER_NAMESPACE;
using namespace CBASE_NAMESPACE;
using namespace NU_NAMESPACE;

#define MAX_MESSAGEBUFF_LEN     1024 * 1024 * 1

NU_SER_BEGIN

bool ThreadsTcpBroWorker::doWork()
{
    CBT_DEBUG("ThreadsTcpBroWorker", "doWork() worker was running");

    SocketBufferContextSPtr sock_buffer_ctx = std::dynamic_pointer_cast<SocketBufferContext>(mContext);
    if (sock_buffer_ctx = NULL)
    {
        CBT_WARN("ThreadsTcpBroWorker()", "doWork() context cast failed(IContext -> SocketBufferContext), "
                << "bro worker will not do work");
        return;
    }

    registerInternelReadCompleteCb(std::bind(&sock_buffer_ctx->onInternelReadCompleteCb, this));
    registerInternelWriteCompleteCb(std::bind(&sock_buffer_ctx->onInternelWriteCompleteCb, this));

    IPV4SocketContextSPtr socket_context = std::dynamic_pointer_cast<IPV4SocketContext>(sock_buffer_ctx->getSocketContext());
    if (socket_context == NULL)
    {
        CBT_WARN("ThreadsTcpBroWorker()", "doWork() context cast failed(IContext -> IPV4SocketContext), "
                << "bro worker will not do work");
        return;
    }

    mRecvThread = boost::thread(boost::bind(&ThreadsTcpBroWorker::handleReadEvent, this, socket_context));
    mSendThread = boost::thread(boost::bind(&ThreadsTcpBroWorker::handleWriteEvent, this, socket_context));

    mRecvThread.join();
    mSendThread.join();

    CBT_DEBUG("ThreadsTcpBroWorker", "doWork() worker was done");

    return true;
}

void ThreadsTcpBroWorker::enableReadEvent()
{
    std::unique_lock<std::mutex> lk(mReadMutex);
    mIsReadEnabled = true;
}

void ThreadsTcpBroWorker::enableWriteEvent()
{
    std::unique_lock<std::mutex> lk(mWriteMutex);
    mIsWriteEnabled = true;
}

void ThreadsTcpBroWorker::disbaleReadEvent()
{
    std::unique_lock<std::mutex> lk(mReadMutex);
    mIsReadEnabled = false;
}

void ThreadsTcpBroWorker::disbaleWriteEvent()
{
    std::unique_lock<std::mutex> lk(mWriteMutex);
    mIsWriteEnabled = false;
}

void ThreadsTcpBroWorker::handleReadEvent(IPV4SocketContextSPtr socket_context)
{
    CBT_DEBUG("ThreadsTcpBroWorker()", "handleReadEvent() start recv thread, client info:" << socket_context->toString());
    ON_SCOPE_EXIT([&]()
            {
                if (socket_context)
                {
                    std::unique_lock<std::mutex> ctx_lk(socket_context->mMutex);
                    close(socket_context->mClientFd);
                    socket_context->mClientFd = INVALID_FD;
                }
                CBT_DEBUG("ThreadsTcpBroWorker()", "handleReadEvent() recv thread was quiting, "
                    << "client info:" << socket_context->toString());
                enableWriteEvent();
                mSendThread.interrupt();
                mWriteCond.notify_one();
            });

    while (true)
    {
        try
        {
            boost::this_thread::interruption_point();
        }
        catch (boost::thread_interrupted&)
        {
            CBT_DEBUG("ThreadsTcpBroWorker()", "handleReadEvent() trigger thread interrupted, send thread ready to quit quiting");
            return;
        }

        std::unique_lock<std::mutex> lk(mReadMutex);
        while (!mIsReadEnabled)
            mReadCond.wait(lk);
        lk.unlock();

        char recv_buffer[MAX_MESSAGEBUFF_LEN] = {0};
        int rb = recv(socket_context->mClientFd, recv_buffer, MAX_MESSAGEBUFF_LEN, 0);
        if (rb < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                CBT_DEBUG("ThreadsTcpBroWorker", "handleReadEvent() EAGAIN or EWOULDBLOCK returned, try recv() again, "
                        << "client info:" << socket_context->toString());
                continue;
            }
            else
            {
                CBT_DEBUG("ThreadsTcpBroWorker", "handleReadEvent() recv data failed, errno code:" << errno << ", "
                        << "client info:" << socket_context->toString());
                break;
            }
        }
        else if (rb == 0)
        {
            CBT_DEBUG("ThreadsTcpBroWorker", "handleReadEvent() connection was closed by peer, "
                    << "client info:" << socket_context->toString());
            break;
        }

        CBT_INFO("ThreadsTcpBroWorker", "handleReadEvent() recv " << rb << " bytes from client:" << socket_context->toString());
        if (LoggerManager::getLogger("SysLog")->isTraceEnabled())
        {
            // hexdump
            CBT_TRACE("ThreadsTcpBroWorker", "handleReadEvent() "
                    << "hexdump " << rb << " bytes "
                    << "from client:" << socket_context->toString() << "\n"
                    << IOHelper::hexdump(reinterpret_cast<unsigned char*>(recv_buffer), rb));
        }

        // notify internel complete
        if (mInternelReadCb)
            mInternelReadCb(recv_buffer, rb);
    }

    return;
}

void ThreadsTcpBroWorker::handleWriteEvent(IPV4SocketContextSPtr socket_context)
{
    CBT_DEBUG("ThreadsTcpBroWorker()", "handleWriteEvent() start send thread, client info:" << socket_context->toString());
    ON_SCOPE_EXIT([&]()
            {
                if (socket_context)
                {
                    std::unique_lock<std::mutex> ctx_lk(socket_context->mMutex);
                    close(socket_context->mClientFd);
                    socket_context->mClientFd = INVALID_FD;
                }
                enableReadEvent();
                CBT_DEBUG("ThreadsTcpBroWorker()", "handleWriteEvent() send thread was quiting, "
                    << "client info:" << socket_context->toString());
                mRecvThread.interrupt();
                mReadCond.notify_one();
            });

    while (true)
    {
        try
        {
            boost::this_thread::interruption_point();
        }
        catch (boost::thread_interrupted&)
        {
            CBT_DEBUG("ThreadsTcpBroWorker()", "handleWriteEvent() trigger thread interrupted, send thread ready to quit quiting");
            return;
        }

        std::unique_lock<std::mutex> lk(mWriteMutex);
        mWriteCond.wait(lk, [&](){return mIsWriteEnabled;});

        mIsWriteEnabled = false;
        lk.unlock();

        std::unique_lock<std::mutex> ctx_lk(socket_context->mResponseMutex);
        auto response = socket_context->mResponse;
        ctx_lk.unlock();

        if (!IOHelper::send_bytes(socket_context->mClientFd, response.data(), response.size(), 0))
        {
            CBT_DEBUG("ThreadsTcpBroWorker()", "handleWriteEvent() failed to send response, "
                    << "response len:" << response.size() << ", "
                    << "response data:" << response.data() << ", "
                    << "client info:" << socket_context->toString());
            return;
        }
    }

    return;
}

NU_SER_END
