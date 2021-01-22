#include <thread>
#include <common/base/logger/Logger.h>
#include <common/base/logger/LoggerManager.h>
#include <NetUtils/services/io_mode/multi_threads/ThreadsTcpBroServer.h>
#include <NetUtils/services/io_mode/multi_threads/ThreadsTcp6BroServer.h>

using namespace NU_SER_NAMESPACE;
using namespace CBASE_NAMESPACE;
using namespace CBASE_LOGGER_NAMESPACE;

int main(int argc, const char* argv[])
{
    LoggerParameter logger_SysLog_param;
    logger_SysLog_param.module_tag = "SysLog";
    logger_SysLog_param.file_path = "/var/log/SysLog";
    logger_SysLog_param.level = "TRACE";
    LoggerSPtr logger_SysLog = std::make_shared<Logger>();
    logger_SysLog->init(logger_SysLog_param);

    CB_INFO("start engine:" << argv[0]);

    std::thread tcp_echo_thread = std::thread(
        []()
        {
            ITcpBrokerServerEngineSPtr bro_server(std::make_shared<ThreadsTcpBroServer>(
                    "echo_engine"
                    , "127.0.0.1"
                    , 9001
                    ));
            bro_server->start();
        });

    std::thread tcp6_echo_thread = std::thread(
        []()
        {
            ITcpBrokerServerEngineSPtr bro_server(std::make_shared<ThreadsTcp6BroServer>(
                    "echo_engine"
                    , "::1"
                    , 9001
                    ));
            bro_server->start();
        });

    tcp_echo_thread.join();
    tcp6_echo_thread.join();

    return 0;
}
