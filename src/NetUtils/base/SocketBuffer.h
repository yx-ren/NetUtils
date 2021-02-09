#ifndef __NET_UTILS_BASE_SOCKET_BUFFER_H__
#define __NET_UTILS_BASE_SOCKET_BUFFER_H__

#include <vector>
#include <list>
#include <NetUtils/SocketContext.h>
#include <NetUtils/base/common.h>
#include <NetUtils/base/Buffer.h>

NU_BASE_BEGIN

class IOEvent
{
public:
    enum EVENT_TYPE
    {
        ET_SYNC_READ,
        ET_SYNC_WRITE,
        ET_ASYNC_READ,
        ET_ASYNC_WRITE,
    };

    IOEvent(SocketBufferContextSPtr ctx)
        : mSockBufCtx(ctx)
        , mIsConsumed(false)
    {}

    bool process()
    {
        ON_SCOPE_EXIT([]() {mIsConsumed = true;} );
        // TODO......

        return true;
    }

    SocketBufferContextSWtr mSockBufCtx;
    bool mIsConsumed;
    EVENT_TYPE mType;
    std::shared_ptr<std::string> mData;
    size_t mLen;
};
typedef std::shared_ptr<IO_EVENT> IOEventSPtr;
typedef std::weak_ptr<IO_EVENT> IOEventWPtr;

enum IO_EVENT_RESULT
{
    IER_SUCCESSED,
    IER_FAILED,
    IER_CLOSED_BY_PEER,
    IER_RST_BY_PEER,
    IER_NOT_ENOUGH_READ_DATA,
    IER_NOT_ENOUGH_WRITE_SPACE,
};

#ifdef __BUFF_OBJECT__
typedef std::function<void(IO_EVENT_RESULT, SocketBufferContextSPtr sbc, BufferSPtr buffer, size_t pos, size_t len)> ExternelIOCompleteCb;
typedef std::function<void(IO_EVENT_RESULT, SocketBufferContextSPtr sbc, BufferSPtr buffer, size_t pos, size_t len)> ExternelReadCompleteCb;
typedef std::function<void(IO_EVENT_RESULT, SocketBufferContextSPtr sbc, BufferSPtr buffer, size_t pos, size_t len)> ExternelWriteCompleteCb;

typedef std::function<void(IO_EVENT_RESULT, SocketBufferContextSPtr sbc, BufferSPtr buffer, size_t pos, size_t len)> InternelIOCompleteCb;
typedef std::function<void(IO_EVENT_RESULT, SocketBufferContextSPtr sbc, BufferSPtr buffer, size_t pos, size_t len)> InternelReadCompleteCb;
typedef std::function<void(IO_EVENT_RESULT, SocketBufferContextSPtr sbc, BufferSPtr buffer, size_t pos, size_t len)> InternelWriteCompleteCb;
#else
typedef std::function<void(IO_EVENT_RESULT result, SocketBufferContextSPtr sbc, const char* data, size_t size)> ExternelIOCompleteCb;
typedef std::function<void(IO_EVENT_RESULT result, SocketBufferContextSPtr sbc, const char* data, size_t size)> ExternelReadCompleteCb;
typedef std::function<void(IO_EVENT_RESULT result, SocketBufferContextSPtr sbc, const char* data, size_t size)> ExternelWriteCompleteCb;

typedef std::function<void(IO_EVENT_RESULT result, SocketBufferContextSPtr sbc, const char* data, size_t size)> InternelIOCompleteCb;
typedef std::function<void(IO_EVENT_RESULT result, SocketBufferContextSPtr sbc, const char* data, size_t size)> InternelReadCompleteCb;
typedef std::function<void(IO_EVENT_RESULT result, SocketBufferContextSPtr sbc, const char* data, size_t size)> InternelWriteCompleteCb;
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
        , mStoreReadBytes(0)
        , mStoreWriteBytes(0)
        , mEventReadBytes(0)
        , mEventWriteBytes(0)
    {}

    SocketBufferContext(SocketContextSPtr context)
        : mSockContext(context)
        , mReadBuffer(nullptr)
        , mWriteBuffer(nullptr)
        , mExternelReadCb(nullptr)
        , mExternelWriteCb(nullptr)
        , mStoreReadBytes(0)
        , mStoreWriteBytes(0)
        , mEventReadBytes(0)
        , mEventWriteBytes(0)
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
     * async io, non block, generate a request(ioevent)
     * when read or write comepleted, the callback function will be triggerd
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

    void setOwner(IOEventLoopThreadWPtr owner);

    IOEventLoopThreadWPtr getOwner();
    const IOEventLoopThreadWPtr getOwner() const;

    const SocketContextSPtr getSocketContext() const;
    SocketContextSPtr getSocketContext();

    void storeKernelRB(const char* data, size_t len); // consider max store data configured by config, TODO......
    void storeKernelWB(const char* data, size_t len);

    std::vector<IOEventSPtr> getReadyIOEvents();

    bool hasReadyEvent();
    bool hasReadyReadEvent();
    bool hasReadyWriteEvent();

    bool processEvent();

protected:

    bool canProcessEvent(IOEvent event);

    bool preProcessEvent(IOEvent event);
    bool postProcessEvent(IOEvent event);

    bool processReadEvent(IOEvent event);
    bool processWriteEvent(IOEvent event);

    IOEventSPtr generateIOEvent(IO_EVENT::EVENT_TYPE type, char* data, size_t len);

private:
    SocketContextSPtr mSockContext;
    BufferSPtr mReadBuffer;
    BufferSPtr mWriteBuffer;
    std::queue<IOEventSPtr> mReadEvents;
    std::queue<IOEventSPtr> mWriteEvents;
    ExternelReadCompleteCb mExternelReadCb;
    ExternelWriteCompleteCb mExternelWriteCb;

    size_t mStoreReadBytes;
    size_t mStoreWriteBytes;

    size_t mEventReadBytes;
    size_t mEventWriteBytes;
};
typedef std::shared_ptr<SocketBufferContext> SocketBufferContextSPtr;
typedef std::weak_ptr<SocketBufferContext> SocketBufferContextWPtr;

class SocketBufferContextReader;
class SocketBufferContextWriter;

NU_BASE_END

#endif
