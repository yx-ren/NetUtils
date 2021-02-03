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

typedef std::function<void(IO_EVENT_RESULT, BufferSPtr buffer, size_t pos, size_t len)> InternelIOCompleteCb;
typedef std::function<void(IO_EVENT_RESULT, BufferSPtr buffer, size_t pos, size_t len)> InternelReadCompleteCb;
typedef std::function<void(IO_EVENT_RESULT, BufferSPtr buffer, size_t pos, size_t len)> InternelWriteCompleteCb;
#else
typedef std::function<void(ASYNC_IO_RESULT result, const char* data, size_t size)> ExternelIOCompleteCb;
typedef std::function<void(ASYNC_IO_RESULT result, const char* data, size_t size)> ExternelReadCompleteCb;
typedef std::function<void(ASYNC_IO_RESULT result, const char* data, size_t size)> ExternelWriteCompleteCb;

typedef std::function<void(ASYNC_IO_RESULT result, const char* data, size_t size)> InternelIOCompleteCb;
typedef std::function<void(ASYNC_IO_RESULT result, const char* data, size_t size)> InternelReadCompleteCb;
typedef std::function<void(ASYNC_IO_RESULT result, const char* data, size_t size)> InternelWriteCompleteCb;
#endif

class SocketBufferContext : public IContext
{
public:
    SocketBufferContext()
        : mSockContext(nullptr)
        , mReadBuffer(nullptr)
        , mWriteBuffer(nullptr)
        , mExternelReadCb(nullptr)
        , mExternelWriteCb(nullptr)
    {}

    SocketBufferContext(SocketContextSPtr context)
        : mSockContext(context)
        , mReadBuffer(nullptr)
        , mWriteBuffer(nullptr)
#if 0
        , mInternelReadCb(nullptr)
        , mInternelWriteCb(nullptr)
#endif
        , mExternelReadCb(nullptr)
        , mExternelWriteCb(nullptr)
    {}

    /*
     * sync io, non block, read or write directly,
     * @param[in] data: a region to read/wirte data
     * @param[in] len : data length
     * @return SUCCESS if success
     */
    bool read(char* data, size_t len);
    bool write(const char* data, size_t len);

    /*
     * async io, non block, when read or write comepleted, the callback function will be triggerd
     * @param[in] data: a region to read/wirte data
     * @param[in] len : data length
     * @return SUCCESS if success
     */
    bool read_async(char* data, size_t len);
    bool write_async(const char* data, size_t len);

    /*
     * async io callback function, when post read/write async request and the request completed
     * the registered callback function will be triggerd
     * @param[in] cb: a region to read/wirte data
     */
    void registerExternelReadCompleteCb(ExternelReadCompleteCb cb);
    void registerExternelWriteCompleteCb(ExternelWriteCompleteCb cb);

    const SocketContextSPtr getSocketContext() const;
    SocketContextSPtr getSocketContext();

#if 0
    /*
     * internel io callback function, when read/write completed on raw socket
     * the registered callback function will be triggerd
     * @param[in] cb: a region to read/wirte data
     * consider hide this for user
     */

    void registerInternelReadCompleteCb(InternelReadCompleteCb cb);
    void registerInternelWriteCompleteCb(InternelWriteCompleteCb cb);
#endif

#if 1
    void onInternelReadCompleteCb(InternelReadCompleteCb cb);
    void onInternelWriteCompleteCb(InternelWriteCompleteCb cb);
#endif

protected:
    void storeKernelRB(const char* data, size_t len);
    void storeKernelWB(const char* data, size_t len);

private:
    SocketContextSPtr mSockContext;
    BufferSPtr mReadBuffer;
    BufferSPtr mWriteBuffer;
#if 0
    InternelReadCompleteCb mInternelReadCb;
    InternelWriteCompleteCb mInternelWriteCb;
#endif
    ExternelReadCompleteCb mExternelReadCb;
    ExternelWriteCompleteCb mExternelWriteCb;
};
typedef std::shared_ptr<ThreadsTcpBroServer> SocketBufferContextSPtr;
typedef std::weak_ptr<ThreadsTcpBroServer> SocketBufferContextWPtr;

class SocketBufferContextReader;
class SocketBufferContextWriter;

NU_BASE_END

#endif
