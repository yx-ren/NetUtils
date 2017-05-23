#include "TcpServer.h"
#include <stdlib.h>
#include <iostream>

#if defined(WIN32) || defined(_WIN32)
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#endif

bool TcpServer::start(void)
{
    if (mFd != INVALID_FD)
    {
        std::cout << "server has been start, "
            << "fd:[" << mFd << "], "
            << "listen port:[" << mPort << "]" << std::endl;

        return true;
    }

    if (!initSocket())
    {
        std::cerr << "server init listening socket failed, "
            << "listen port:[" << mPort << "]" << std::endl;

        return false;
    }

    if (!run())
    {
        std::cerr << "call run() failed, "
            << "fd:[" << mFd << "], "
            << "listen port:[" << mPort << "]" << std::endl;

        return false;
    }

    return true;
}

bool TcpServer::stop(void)
{
    close(mFd);
    mFd = INVALID_FD;

    return true;
}
