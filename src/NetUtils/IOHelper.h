#include <string>
#include <NetUtils/common.h>

#ifndef __NET_UTILS_IO_HEPLER_H__
#define __NET_UTILS_IO_HEPLER_H__

NU_BEGIN

class IOHelper
{
public:
    static bool send_bytes(int sockfd, const void* buf, size_t len, int flags);

    static bool recv_bytes(int sockfd, void* buf, size_t len, int flags);

    static std::string hexdump(const void* buffer, size_t len);
};

NU_END

#endif
