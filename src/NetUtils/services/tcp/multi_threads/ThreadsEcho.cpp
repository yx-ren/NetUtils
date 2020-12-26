#include <iostream>
#include <stdlib.h>

#include <log4cxx/helpers/properties.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/asyncappender.h>
#include <log4cxx/fileappender.h>
#include <log4cxx/level.h>
#include <log4cxx/consoleappender.h>
#include <log4cxx/patternlayout.h>

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
#if 0
    //init_log();
    LoggerParameter logger_param;
    Logger logger;
    logger.init(logger_param);
    LOG4CXX_INFO(logger.get_logger(), "this is a log generate by cbase");
#else
    init_log();
    CB_INFO("this is a log generate by cbase");
#endif

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
