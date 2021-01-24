#include <vector>
#include <list>
#include <NetUtils/SocketContext.h>
#include <NetUtils/base/Buffer.h>

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

//typedef std::function<void(IO_EVENT_RESULT, BufferSPtr buffer, size_t pos, size_t len)> IOEventCallback
typedef std::function<void(IO_EVENT_RESULT, const char* data, size_t len)> IOEventCallback

class IOBuffer
{
public:
    IOBuffer(SocketContextPtr context)
        : mSockContext(context)
    {}

    void addReadEventCB(IOEventCallback cb);
    void addWriteEventCB(IOEventCallback cb);

    void removeReadEventCB(IOEventCallback cb);
    void removeWriteEventCB(IOEventCallback cb);

    bool read_async(size_t len);
    bool write_async(const char* data, size_t len);

    bool read(char* data, size_t len);
    bool write(const char* data, size_t len);

private:
    SocketContextPtr mSockContext;
    BufferSPtr mReadBuffer;
    BufferSPtr mWriteBuffer;
    std::queue<IO_EVENT> mIOEvents;

    std::list<IOEventCallback> mReadEventCBs;
    std::list<IOEventCallback> mWriteEventCBs;
};

class IOBufferReader;
class IOBufferWriter;
