#ifndef __NET_UTILS_BASE_BUFFER_H__
#define __NET_UTILS_BASE_BUFFER_H__

#include <vector>
#include <NetUtils/base/common.h>

//#define __BUFF_OBJECT__

NU_BASE_BEGIN

class Buffer
{
public:
    Buffer()
        : pos(0)
    {}

    bool read(char* data, size_t len);
    bool write(const char* data, size_t len);

    char operator[] (size_t n) const
    {
        return mBuffers[n];
    }

    char* getRawBuffer
    {
        return mBuffers.data();
    }

private:
    std::vector<char> mBuffers;
    int pos
};
typedef std::shared_ptr<Buffer> BufferSPtr;
typedef std::weak_ptr<Buffer> BufferWPtr;

class BufferReader;
class BufferWriter;

NU_BASE_END

#endif
