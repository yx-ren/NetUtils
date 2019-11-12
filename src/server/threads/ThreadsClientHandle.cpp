#include "ThreadsClientHandle.h"

#include <iostream>
#include <ostream>
#include <sstream>
#include <iomanip>
#include <string.h>

#define BACK_LOG                10
#define MAX_MESSAGEBUFF_LEN     0xffff

std::string ThreadsClientHandle::dumpHex(const char* buf, int len)
{
    std::ostringstream oss;
    for (int i = 0; i != len; i++)
    {
        oss << "0x" << std::setw(2) << std::setfill('0') << std::hex << ((int)buf[i] & 0xff) << " ";
        if (((i + 1) % 0x10) == 0)
            oss << std::endl;
    }

    return oss.str();
}
