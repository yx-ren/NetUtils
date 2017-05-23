#include "IPV4ThreadsTcpServer.h"

#include <iostream>
#include <string.h>

#define BACK_LOG                10
#define MAX_MESSAGEBUFF_LEN     0xffff

void IPV4ThreadsClientHandle::doWork(void)
{
    std::thread threadHandleRead(std::bind(&IPV4ThreadsClientHandle::handleReadEvent, this));
    std::thread threadHandleWrite(std::bind(&IPV4ThreadsClientHandle::handleWriteEvent, this));

    threadHandleRead.join();
    threadHandleWrite.join();

    return;
}

void IPV4ThreadsClientHandle::handleReadEvent(void)
{
    IPV4SocketContextPtr socket_context = std::dynamic_pointer_cast<IPV4SocketContext>(mContext);
    if (socket_context == NULL)
    {
        std::cerr << "SocketContextPtr -> IPV4SocketContextPtr failed" << std::endl;
        throw std::runtime_error("invalid cast");
    }

    std::cout << "start handle read event thread, "
        << "client fd:[" << socket_context->mClientFd << "], "
        << "client addr:[" << inet_ntoa(socket_context->mClientAddr.sin_addr) << "], "
        << "client port:[" << std::dec << ntohs(socket_context->mClientAddr.sin_port) << "]" << std::endl;

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

        std::cout << "server: recv from address:[" << inet_ntoa(socket_context->mClientAddr.sin_addr) << "], "
            << "port:[" << std::dec << ntohs(socket_context->mClientAddr.sin_port) << "], "
            << "bytes:[" << rb << "],"
            << "msg:[" << recv_msg << "], "
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

void IPV4ThreadsClientHandle::handleWriteEvent(void)
{
    IPV4SocketContextPtr socket_context = std::dynamic_pointer_cast<IPV4SocketContext>(mContext);
    if (socket_context == NULL)
    {
        std::cerr << "SocketContextPtr -> IPV4SocketContextPtr failed" << std::endl;
        throw std::runtime_error("invalid cast");
    }

    std::cout << "start handle write event thread, "
        << "client fd:[" << socket_context->mClientFd << "], "
        << "client addr:[" << inet_ntoa(socket_context->mClientAddr.sin_addr) << "], "
        << "client port:[" << std::dec << ntohs(socket_context->mClientAddr.sin_port) << "]" << std::endl;

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

            std::cout << "server: send to address:[" << inet_ntoa(socket_context->mClientAddr.sin_addr) << "], "
                << "port:[" << ntohs(socket_context->mClientAddr.sin_port) << "], "
                << "bytes:[" << sb << "],"
                << "msg:[" << send_msg << "], "
                << "hex buf:\n"
                << dumpHex(send_msg->c_str(), send_msg->size()) << std::endl << std::endl;

            total_send += sb;
        }
    }

    close(socket_context->mClientFd);
    socket_context->mClientFd = INVALID_FD;

    return;
}

bool IPV4ThreadsTcpServer::initSocket(void)
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
    int svr_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (svr_sock < 0)
    {
        std::cerr << "server: call socket() failed, "
            << "err code:[" << GET_LAST_SOCKET_ERROR << "]" << std::endl;
        return false;
    }

    int option = 1;
    if (setsockopt(svr_sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&option, sizeof(option)) < 0)
    {
        std::cerr << "server: call setsockopt() failed, "
            << "err code:[" << GET_LAST_SOCKET_ERROR << "]" << std::endl;

        return false;
    }

    std::cout << "server: create ipv4 tcp server socket......" << std::endl;

    // -------------------- //

    // init sockaddr_in struct
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family      = AF_INET;
    server_addr.sin_addr.s_addr = mIp.empty() ? INADDR_ANY : inet_addr(mIp.c_str());
    server_addr.sin_port        = htons(mPort);

    // bind
    if (bind(svr_sock, (sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        std::cerr << "server: call bind() failed, "
            << "err code:[" << GET_LAST_SOCKET_ERROR << "]" << std::endl;
        return false;
    }

    std::cout << "server: bind the local address, "
        << "ip:[" << (mIp.empty() ? "127.0.0.1" : mIp) << "], "
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

bool IPV4ThreadsTcpServer::run(void)
{
    while (true)
    {
        struct sockaddr_in cli_addr;
        socklen_t ipv4addr_len = sizeof(sockaddr_in);
        int client_socket = accept(mFd, (sockaddr*)&cli_addr, &ipv4addr_len);
        if (client_socket < 0)
        {
            std::cerr << "server: call accept() failed, "
                << "err code:[" << GET_LAST_SOCKET_ERROR << "]" << std::endl;
            return false;
        }

        std::cout << "server: accept from "
            << ":[" << inet_ntoa(cli_addr.sin_addr) << "]:[" << ntohs(cli_addr.sin_port) << "] client" << std::endl;

        IPV4SocketContextPtr context(new IPV4SocketContext(cli_addr, client_socket));
        std::shared_ptr<std::thread> threadHandleClient(new std::thread(std::bind(&IPV4ThreadsTcpServer::handleClient, this, context)));
        threadHandleClient->detach();
    }

    return true;
}

void IPV4ThreadsTcpServer::handleClient(SocketContextPtr context)
{
    std::cout << "server: thread:[" << std::hex << std::this_thread::get_id() << "], "
        << "provide service" << std::endl;

    //ThreadsClientHandlePtr clientHandle(new IPV4ThreadsClientHandle(context));
    ThreadsClientHandlePtr clientHandle(new IPV4ThreadsClientHandle(std::dynamic_pointer_cast<IPV4SocketContext>(context)));
    clientHandle->doWork();

    std::cout << "server: thread:[" << std::hex << std::this_thread::get_id() << "], "
        << "start to closed" << std::endl;

    return;
}
