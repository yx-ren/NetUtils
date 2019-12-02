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

    auto time_begin = std::chrono::high_resolution_clock::now();
    for (int i = 0; i != 500000; i++)
    {
        std::ostringstream oss;
        oss << "now is:[" << i << "] tick";
        std::string log_info = oss.str();
        logger.writeLog(log_info, LOG_LEVEL_DEBUG);
        //std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    auto time_end = std::chrono::high_resolution_clock::now();
    auto cost_us = std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_begin);
    std::cout << "cost time:[" << cost_us.count() << "] ms" << std::endl;

    return 0;
}
