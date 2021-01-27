#ifndef __NET_UTILS_BASE_SOCKET_BUFFER_H__
#define __NET_UTILS_BASE_SOCKET_BUFFER_H__

#include <vector>
#include <list>
#include <NetUtils/SocketContext.h>
#include <NetUtils/base/common.h>
#include <NetUtils/base/Buffer.h>

NU_BASE_BEGIN
class IO_EVENT
{
public:
    enum EVENT_TYPE
    {
        ET_SYNC_READ,
        ET_SYNC_WRITE,
        ET_ASYNC_READ,
        ET_ASYNC_WRITE,
    };

    std::shared_ptr<std::string> data;
};

enum IO_EVENT_RESULT
{
    IER_SUCCESSED,
    IER_FAILED,
};

#ifdef __BUFF_OBJECT__
typedef std::function<void(IO_EVENT_RESULT, BufferSPtr buffer, size_t pos, size_t len)> ExternelIOCompleteCb;
typedef std::function<void(IO_EVENT_RESULT, BufferSPtr buffer, size_t pos, size_t len)> ExternelReadCompleteCb;
typedef std::function<void(IO_EVENT_RESULT, BufferSPtr buffer, size_t pos, size_t len)> ExternelWriteCompleteCb;
#else
typedef std::function<void(ASYNC_IO_RESULT result, const char* data, size_t size)> ExternelIOCompleteCb;
typedef std::function<void(ASYNC_IO_RESULT result, const char* data, size_t size)> ExternelReadCompleteCb;
typedef std::function<void(ASYNC_IO_RESULT result, const char* data, size_t size)> ExternelWriteCompleteCb;
#endif

class SocketBuffer
{
public:
    SocketBuffer(SocketContextPtr context)
        : mSockContext(context)
    {}

    bool read(char* data, size_t len);
    bool write(const char* data, size_t len);

    bool read_async(size_t len);
    bool write_async(const char* data, size_t len);

    void storeKernelRB(const char* data, size_t len);
    void storeKernelWB(const char* data, size_t len);

private:
    SocketContextPtr mSockContext;
    BufferSPtr mReadBuffer;
    BufferSPtr mWriteBuffer;
};
typedef std::shared_ptr<ThreadsTcpBroServer> SocketBufferSPtr;
typedef std::weak_ptr<ThreadsTcpBroServer> SocketBufferWPtr;

class SocketBufferReader;
class SocketBufferWriter;

NU_BASE_END

#endif