#ifndef __NET_UTILS_PROTOCOL_H__
#define __NET_UTILS_PROTOCOL_H__

#include <string>
#include <NetUtils/common.h>

NU_BEGIN

class protocol
{
public:
    enum PROTOCOL_TYPE
    {
        PT_TCP_IPV4,
        PT_TCP_IPV6,
        PT_UDP_IPV4,
        PT_UDP_IPV6,
    };

    static std::string toString(PROTOCOL_TYPE type)
    {
        switch(type)
        {
        case PT_TCP_IPV4:
            return "tcp";
            break;

        case PT_TCP_IPV6:
            return "tcp6";
            break;

        case PT_UDP_IPV4:
            return "udp";
            break;

        case PT_UDP_IPV6:
            return "udp6";
            break;
        }

        return "unknown protocol type";
    }
};

NU_END

#endif
