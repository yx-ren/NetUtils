#include "IPV6ThreadsTcpServer.h"

#include <iostream>
#include <string.h>

void IPV6ThreadsClientHandle::doWork(void)
{
    std::thread threadHandleRead(std::bind(&IPV6ThreadsClientHandle::handleReadEvent, this));
    std::thread threadHandleWrite(std::bind(&IPV6ThreadsClientHandle::handleWriteEvent, this));

    threadHandleRead.join();
    threadHandleWrite.join();

    return;
}

void IPV6ThreadsClientHandle::handleReadEvent(void)
{
    char buf[0xff] = {0};
    IPV6SocketContextPtr socket_context = std::dynamic_pointer_cast<IPV6SocketContext>(mContext);
    if (socket_context == NULL)
    {
        std::cerr << "SocketContextPtr -> IPV6SocketContextPtr failed" << std::endl;
        throw std::runtime_error("invalid cast");
    }

    std::cout << "start handle read event thread, "
        << "client fd:[" << socket_context->mClientFd << "], "
        //<< "client addr:[" << inet_ntoa(socket_context->mClientAddr.sin_addr) << "], "
        << "client addr:[" << inet_ntop(AF_INET6, &socket_context->mClientAddr.sin6_addr, buf, sizeof(buf)) << "], "
        << "client port:[" << std::dec << ntohs(socket_context->mClientAddr.sin6_port) << "]" << std::endl;

    while (true)
    {
        char recv_msg_buf[MAX_MESSAGEBUFF_LEN] = {0};
        int rb = recv(socket_context->mClientFd, recv_msg_buf, MAX_MESSAGEBUFF_LEN, 0);
        if (rb < 0)
        {
#ifndef WIN32
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                std::cout << "try again" << std::endl;
                continue;
            }
#endif
            std::cerr << "server: call recv() failed, "
                << "err code:[" << GET_LAST_SOCKET_ERROR << "]" << std::endl;
            break;
        }
        else if (rb == 0)
        {
            std::cout << "server: socket closed by peer" << std::endl;
            break;
        }

        std::shared_ptr<std::string> recv_msg(new std::string(recv_msg_buf));

        std::cout << "server: recv from "
            << "address:[" << inet_ntop(AF_INET6, &socket_context->mClientAddr.sin6_addr, buf, sizeof(buf)) << "], "
            << "port:[" << std::dec << ntohs(socket_context->mClientAddr.sin6_port) << "], "
            << "bytes:[" << rb << "], "
            << "msg:[" << *recv_msg << "], "
            << "hex buf:\n"
            << dumpHex(recv_msg->c_str(), recv_msg->size()) << std::endl << std::endl;

        std::unique_lock<std::mutex> lk(mMutex);
        mMessageQueue.push(recv_msg);
        lk.unlock();

        mCond.notify_one();
    }

    close(socket_context->mClientFd);
    socket_context->mClientFd = INVALID_FD;
    mCond.notify_one();

    return;
}

void IPV6ThreadsClientHandle::handleWriteEvent(void)
{
    IPV6SocketContextPtr socket_context = std::dynamic_pointer_cast<IPV6SocketContext>(mContext);
    if (socket_context == NULL)
    {
        std::cerr << "SocketContextPtr -> IPV6SocketContextPtr failed" << std::endl;
        throw std::runtime_error("invalid cast");
    }

    char buf[0xff] = {0};
    std::cout << "start handle write event thread, "
        << "client fd:[" << socket_context->mClientFd << "], "
        << "client addr:[" << inet_ntop(AF_INET6, &socket_context->mClientAddr.sin6_addr, buf, sizeof(buf)) << "], "
        << "client port:[" << std::dec << ntohs(socket_context->mClientAddr.sin6_port) << "]" << std::endl;

    while (true)
    {
        std::unique_lock<std::mutex> lk(mMutex);
        if (mMessageQueue.empty())
            mCond.wait(lk);

        if (socket_context->mClientFd == INVALID_FD)
        {
            std::cout << "the socket has been closed, exit..." << std::endl;
            break;
        }

        std::shared_ptr<std::string> send_msg = mMessageQueue.front();
        mMessageQueue.pop();
        lk.unlock();

        size_t total_send = 0;
        while (total_send < send_msg->size())
        {
            size_t sb = send(socket_context->mClientFd, send_msg->data() + total_send, send_msg->size() - total_send, 0);
            if (sb < 0)
            {
#ifndef WIN32
                if (errno == EAGAIN || errno == EWOULDBLOCK)
                {
                    std::cout << "try again" << std::endl;
                    continue;
                }
#endif
                std::cerr << "server: call send() failed, "
                    << "err code:[" << GET_LAST_SOCKET_ERROR << "]" << std::endl;
                break;
            }
            else if (sb == 0)
            {
                std::cerr << "server: call send() failed, "
                    << "err code:[" << GET_LAST_SOCKET_ERROR << "]" << std::endl;
                break;
            }

            std::cout << "server: send to "
                << "address:[" << inet_ntop(AF_INET6, &socket_context->mClientAddr.sin6_addr, buf, sizeof(buf)) << "], "
                << "port:[" << ntohs(socket_context->mClientAddr.sin6_port) << "], "
                << "bytes:[" << sb << "], "
                << "msg:[" << *send_msg << "], "
                << "hex buf:\n"
                << dumpHex(send_msg->c_str(), send_msg->size()) << std::endl << std::endl;

            total_send += sb;
        }
    }

    close(socket_context->mClientFd);
    socket_context->mClientFd = INVALID_FD;

    return;
}

bool IPV6ThreadsTcpServer::initSocket(void)
{
#ifdef WIN32
    // init win32 socket
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        std::cerr << "server: call WSAStartup() failed, "
            << "err code:[" << GET_LAST_SOCKET_ERROR << "]" << std::endl;
        return false;
    }
#endif

    // create server socket
    int svr_sock = socket(AF_INET6, SOCK_STREAM, 0);
    if (svr_sock < 0)
    {
        std::cerr << "server: call socket() failed, "
            << "err code:[" << GET_LAST_SOCKET_ERROR << "]" << std::endl;
        return false;
    }

    int option = 1;
    if (setsockopt(svr_sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&option, sizeof(option)) < 0)
    {
        std::cerr << "server: call setsockopt() failed, for [SO_REUSEADDR], "
            << "err code:[" << GET_LAST_SOCKET_ERROR << "]" << std::endl;

        return false;
    }

    if (setsockopt(svr_sock, IPPROTO_IPV6, IPV6_V6ONLY, (const char*)&option, sizeof(option)) < 0)
    {
        std::cerr << "server: call setsockopt() failed, for [IPV6_V6ONLY], "
            << "err code:[" << GET_LAST_SOCKET_ERROR << "]" << std::endl;

        return false;
    }

    std::cout << "server: create ipv6 tcp server socket......" << std::endl;

    // -------------------- //

    // init sockaddr_in struct
    struct sockaddr_in6 server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin6_family      = AF_INET6;
    server_addr.sin6_port        = htons(mPort);
    if (mIp.empty())
        server_addr.sin6_addr = in6addr_any;
    else
    {
        if (inet_pton(AF_INET6, mIp.c_str(), &server_addr.sin6_addr) != 1)
        {
            std::cerr << "server: call inet_pton() failed, ip:[" << mIp << "]" << std::endl;
            return -1;
        }
    }

    // bind
    if (bind(svr_sock, (sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        std::cerr << "server: call bind() failed, "
            << "err code:[" << GET_LAST_SOCKET_ERROR << "]" << std::endl;
        return false;
    }

    std::cout << "server: bind the local address, "
        << "ip:[" << (mIp.empty() ? "::1" : mIp) << "], "
        << "port:[" << std::dec << mPort << "]" << std::endl;

    // -------------------- //

    // listen
    if (listen(svr_sock, BACK_LOG) < 0)
    {
        std::cerr << "server: call listen() failed, "
            << "listen port:[" << mPort << "], "
            << "err code:[" << GET_LAST_SOCKET_ERROR << "]" << std::endl;
        return false;
    }

    mFd = svr_sock;

    return true;
}

bool IPV6ThreadsTcpServer::run(void)
{
    while (true)
    {
        struct sockaddr_in6 cli_addr;
        socklen_t ipv6addr_len = sizeof(sockaddr_in6);
        int client_socket = accept(mFd, (struct sockaddr*)&cli_addr, &ipv6addr_len);
        if (client_socket < 0)
        {
            std::cerr << "server: call accept() failed, "
                << "err code:[" << GET_LAST_SOCKET_ERROR << "]" << std::endl;
            return false;
        }

        char buf[0xff] = {0};
        std::cout << "server: accept from "
            << ":[" << inet_ntop(AF_INET6, &cli_addr.sin6_addr, buf, sizeof(buf)) << "]"
            //<< ":[" << cli_addr.sin6_port << "] client" << std::endl;
            << ":[" << ntohs(cli_addr.sin6_port) << "] client" << std::endl;

        IPV6SocketContextPtr context(new IPV6SocketContext(cli_addr, client_socket));
        std::shared_ptr<std::thread> threadHandleClient(new std::thread(
            std::bind(&IPV6ThreadsTcpServer::handleClient, this, context)));
        threadHandleClient->detach();
    }

    return true;
}

void IPV6ThreadsTcpServer::handleClient(SocketContextPtr context)
{
    std::cout << "server: thread:[" << std::hex << std::this_thread::get_id() << "], "
        << "provide service" << std::endl;

    ThreadsClientHandlePtr clientHandle(new IPV6ThreadsClientHandle(
        std::dynamic_pointer_cast<IPV6SocketContext>(context)));
    clientHandle->doWork();

    std::cout << "server: thread:[" << std::hex << std::this_thread::get_id() << "], "
        << "start to closed" << std::endl;

    return;
}
