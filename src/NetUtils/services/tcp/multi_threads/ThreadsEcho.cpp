#include <iostream>
#include <stdlib.h>

#include <common/base/logger/Logger.h>
#include <common/base/logger/LoggerManager.h>

#include <NetUtils/services/common.h>
#include <NetUtils/services/tcp/multi_threads/IPV4ThreadsTcpServer.h>
#include <NetUtils/services/tcp/multi_threads/IPV6ThreadsTcpServer.h>

using namespace CBASE_LOGGER_NAMESPACE;
using namespace NU_SER_NAMESPACE;

void init_log()
{
    LoggerParameter log_param;
    log_param.module_tag = "SysLog";
    log_param.file_path = "../var/log/multi_threads_echo.log";
    Logger logger;
    logger.init(log_param);

    LoggerManager::addLogger(logger.getRawLogger());
}

int main(int argc, const char* argv[])
{
    init_log();
    CB_INFO("this is a log generate by cbase");

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
