#include <iostream>
#include <stdlib.h>
#include <log4cxx/helpers/properties.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/asyncappender.h>
#include <log4cxx/fileappender.h>
#include <log4cxx/level.h>
#include <log4cxx/consoleappender.h>
#include <log4cxx/patternlayout.h>
#include "IPV4ThreadsTcpServer.h"
#include "IPV6ThreadsTcpServer.h"
#include <common/base/logger/Logger.h>
using namespace CBASE_LOGGER_NAMESPACE;

void init_log()
{
    using namespace log4cxx;
    using namespace log4cxx::helpers;

    Properties props;
    props.put("log4j.logger.SysLog", "INFO, console");
    props.put("log4j.appender.console", "org.apache.log4j.ConsoleAppender");
    props.put("log4j.appender.console.layout", "org.apache.log4j.PatternLayout");
    props.put("log4j.appender.console.layout.ConversionPattern", "%d [%t] %-5p %.32c - %m%n");

    PropertyConfigurator::configure(props);

    auto logger = log4cxx::Logger::getLogger("SysLog");
    LOG4CXX_INFO(logger, "this is a log generate by log4cxx");
}

int main(int argc, const char* argv[])
{
    //init_log();
    LoggerParameter logger_param;
    Logger logger;
    logger.init(logger_param);
    LOG4CXX_INFO(logger.get_logger(), "this is a log generate by cbase");

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
