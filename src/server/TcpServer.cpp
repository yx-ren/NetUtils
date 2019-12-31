#include "TcpServer.h"
#include <stdlib.h>
#include <iostream>

#if defined(WIN32) || defined(_WIN32)
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#endif

TcpServer::TcpServer(uint16_t port)
    : mIp(""), mPort(port), mFd(INVALID_FD)
{}

TcpServer::TcpServer(const std::string ip, uint16_t port)
    : mIp(ip), mPort(port), mFd(INVALID_FD)
{}

TcpServer::~TcpServer()
{}

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
    CLOSE_SOCKET(mFd);
    mFd = INVALID_FD;

    return true;
}

void TcpServer::setListenSockCBs(SetSockOptCBsVectorPtr listenSockCBs)
{
    mListenSockCBs = listenSockCBs;
}

void TcpServer::setAcceptSockCBs(SetSockOptCBsVectorPtr acceptSockCBs)
{
    mAcceptSockCBs = acceptSockCBs;
}

void TcpServer::applyListenSockCBs(int listenSock)
{
    for (const auto& cb : *mListenSockCBs)
        (*cb)(listenSock);

    return;
}

void TcpServer::applyAcceptSockCBs(int acceptSock)
{
    for (const auto& cb : *mAcceptSockCBs)
        (*cb)(acceptSock);

    return;
}
