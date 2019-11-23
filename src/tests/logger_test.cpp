#include <iostream>
#include <ostream>
#include <sstream>
#include <thread>
#include <chrono>
#include <logger/Logger.h>

int main(int argc, const char* argv[])
{
    LogConfig log_conf;
    log_conf.mode = LOG_MODE_FILE;
    Logger logger(log_conf);
    std::cout << LoggerHelper::configToString(log_conf) << std::endl;
    if (!logger.init())
    {
        std::cerr << "call Logger::init() failed" << std::endl;
        return -1;
    }

    for (int i = 0; i != 500000; i++)
    {
        std::ostringstream oss;
        oss << "now is:[" << i << "] tick";
        std::string log_info = oss.str();
        logger.writeLog(log_info, LOG_LEVEL_DEBUG);
        //std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
