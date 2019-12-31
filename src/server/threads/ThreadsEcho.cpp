#include <iostream>
#include <stdlib.h>
#include "IPV4ThreadsTcpServer.h"
#include "IPV6ThreadsTcpServer.h"

int main(int argc, const char* argv[])
{
    // check input arguments
    if (argc < 2)
    {
        std::cerr << "usage: " << argv[0] << " "
            << "[port] " << std::endl
            << "[host] (default is localhost)" << std::endl;
        return -1;
    }

    TcpServerPtr server;
    if (argc >= 3)
        server.reset(new IPV4ThreadsTcpServer(argv[2], atoi(argv[1])));
    else
        server.reset(new IPV4ThreadsTcpServer(atoi(argv[1])));

    if (!server)
    {
        std::cerr << "invalid server pointer" << std::endl;
        return -1;
    }
    std::thread ipv4ServerThread(std::bind(&TcpServer::start, server));

    if (argc >= 3)
        server.reset(new IPV6ThreadsTcpServer(argv[2], atoi(argv[1])));
    else
        server.reset(new IPV6ThreadsTcpServer(atoi(argv[1])));

    if (!server)
    {
        std::cerr << "invalid server pointer" << std::endl;
        return -1;
    }
    std::thread ipv6ServerThread(std::bind(&TcpServer::start, server));

    ipv4ServerThread.join();
    ipv6ServerThread.join();

    std::cout << "server done" << std::endl;

    return 0;
}
