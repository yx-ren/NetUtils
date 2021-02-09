#include <NetUtils/base/SocketBuffer.h>

NU_BASE_BEGIN

bool SocketBufferContext::read(char* data, size_t len)
{
    // TODO......
    return true;
}

bool SocketBufferContext::write(const char* data, size_t len)
{
    // TODO......
    return true;
}

bool SocketBufferContext::read_async(char* data, size_t len)
{
    // TODO......
    mEventReadBytes += len;
    mReadEvents.push_back(generateIOEvent(ET_ASYNC_READ, data, len));
    return true;
}

bool SocketBufferContext::write_async(const char* data, size_t len)
{
    // TODO......
    mEventWriteBytes += len;
    mWriteEvents.push_back(generateIOEvent(ET_ASYNC_WRITE, data, len));
    return true;
}

void SocketBufferContext::registerExternelReadCompleteCb(ExternelReadCompleteCb cb)
{
    mExternelReadCb = cb;
}

void SocketBufferContext::registerExternelWriteCompleteCb(ExternelWriteCompleteCb cb)
{
    mExternelWriteCb = cb;
}

const SocketContextSPtr SocketBufferContext::getSocketContext() const
{
    return mSockContext;
}

SocketContextSPtr SocketBufferContext::getSocketContext()
{
    return mSockContext;
}

void SocketBufferContext::storeKernelRB(const char* data, size_t len)
{
    // TODO......
}

void SocketBufferContext::storeKernelWB(const char* data, size_t len)
{
    // TODO......
}

bool SocketBufferContext::hasReadyEvent()
{
    return hasReadyReadEvent() || hasReadyWriteEvent();
}

bool SocketBufferContext::hasReadyReadEvent()
{
    return mStoreReadBytes >= mEventReadBytes;
}

bool SocketBufferContext::hasReadyWriteEvent()
{
    // TODO......
    return mEventWriteBytes > 0;
}

bool SocketBufferContext::canProcessEvent(IOEvent event)
{
    return true;
}

bool SocketBufferContext::preProcessEvent(IOEvent event)
{
    return true;
}

bool SocketBufferContext::processEvent(IOEvent event)
{
    bool process_successed = false;
    switch(event->type)
    {
        case read:
            process_successed = processReadEvent(event);
            break;
        case write:
            process_successed = processWriteEvent(event);
            break;
    }

    return process_successed;
}

bool SocketBufferContext::postProcessEvent(IOEvent event)
{
    return true;
}

bool SocketBufferContext::processReadEvent(IOEvent event)
{
    //mExternelReadCompleteCb();
    return true;
}

bool SocketBufferContext::processWriteEvent(IOEvent event)
{
    //mExternelWriteCompleteCb();
    return true;
}
#

NU_BASE_END
