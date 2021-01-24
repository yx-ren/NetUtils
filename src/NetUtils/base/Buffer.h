#include <vector>

class Buffer
{
public:
    Buffer()
        : pos(0)
    {}

private:
    std::vector<unsigned char> mBuffers;
    int pos
}
typedef std::shared_ptr<Buffer> BufferSPtr;
typedef std::weak_ptr<Buffer> BufferWPtr;

class BufferReader;
class BufferWriter;
