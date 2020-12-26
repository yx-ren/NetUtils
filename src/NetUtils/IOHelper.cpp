#include <NetUtils/IOHelper.h>
#if defined(WIN32) || defined (_WIN32)
#include <ws2tcpip.h>
#include <WinSock.h>
#include <process.h>
#include <mstcpip.h>
#else
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif
#include <common/base/logger/Logger.h>
#include <common/base/logger/LoggerManager.h>
#include <NetUtils/services/common.h>

using namespace CBASE_LOGGER_NAMESPACE;

NU_BEGIN

bool IOHelper::send_bytes(int sockfd, const void* buf, size_t len, int flags)
{
    const unsigned char* data = reinterpret_cast<const unsigned char*>(buf);
    size_t total_send = 0;
    while (total_send < len)
    {
        size_t left_len = len - total_send;
        size_t sb = send(sockfd,
                data + total_send, left_len, flags);
        if (sb < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                CBT_DEBUG("IOHelper", "send_bytes() EAGAIN or EWOULDBLOCK returned, try send() again, fd:" << sockfd);
                continue;
            }
            else
            {
                CBT_DEBUG("IOHelper", "send_bytes() send data failed, errno code:" << errno << ", fd:" << sockfd);
                return false;
            }
        }
        else if (sb == 0)
        {
            CBT_DEBUG("IOHelper", "send_bytes() connection was closed by peer, fd:" << sockfd);
            return false;
        }

        total_send += sb;
        if (LoggerManager::getLogger("SysLog")->isTraceEnabled())
        {
            CBT_TRACE("IOHelper", "send_bytes() "
                    << "hexdump " << sb << " bytes "
                    << "from fd:" << sockfd << "\n"
                    << hexdump(data, sb));
        }
    }

    return true;
}

bool IOHelper::recv_bytes(int sockfd, void* buf, size_t len, int flags)
{
    unsigned char* data = reinterpret_cast<unsigned char*>(buf);
    size_t total_rb = 0;
    while (total_rb < len)
    {
        int rb = recv(sockfd, data + total_rb, len - total_rb, flags);
        if (rb < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                CBT_DEBUG("IOHelper", "recv_bytes() EAGAIN or EWOULDBLOCK returned, try recv() again, fd:" << sockfd);
                continue;
            }
            else
            {
                CBT_DEBUG("IOHelper", "recv_bytes() recv data failed, errno code:" << errno << ", fd:" << sockfd); return false;
            }
        }
        else if (rb == 0)
        {
            CBT_DEBUG("IOHelper", "recv_bytes() connection was closed by peer, fd:" << sockfd);
            return false;
        }

        total_rb += rb;
        CBT_INFO("IOHelper", "recv_bytes() recv " << rb << " bytes from fd:" << sockfd);
        if (LoggerManager::getLogger("SysLog")->isTraceEnabled())
        {
            CBT_TRACE("IOHelper", "recv_bytes() "
                    << "hexdump " << rb << " bytes "
                    << "from fd:" << sockfd << "\n"
                    << hexdump(data, rb));
        }
    }

    return true;
}

std::string IOHelper::hexdump(const void* buffer, size_t len)
{
    const unsigned char* data = reinterpret_cast<const unsigned char*>(buffer);
    std::ostringstream oss;
    for (int i = 0; i != len; i++)
    {
        oss << "0x" << std::setw(2) << std::setfill('0') << std::hex << ((int)data[i] & 0xff) << " ";
        if (((i + 1) % 0x10) == 0)
            oss << std::endl;
    }

    return oss.str();
}

NU_END
