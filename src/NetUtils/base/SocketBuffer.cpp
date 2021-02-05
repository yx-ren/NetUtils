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
    getOwner()->addIOEvent(generateIOEvent());
    return true;
}

bool SocketBufferContext::write_async(const char* data, size_t len)
{
    // TODO......
    getOwner()->addIOEvent(generateIOEvent());
    return true;
}

void SocketBufferContext::registerExternelReadCompleteCb(ExternelReadCompleteCb cb)
{
    // TODO......
}

void SocketBufferContext::registerExternelWriteCompleteCb(ExternelWriteCompleteCb cb)
{
    // TODO......
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

NU_BASE_END
