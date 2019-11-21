#include "Logger.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <mutex>

Logger::Logger()
{
    mImpl.reset(new LoggerImpl);
}

bool Logger::init(const LogConfig& conf)
{
    return mImpl->init();
}

std::string Logger::configToString(void)
{
    return mImpl->configToString(void);
}

class LoggerImpl
{
public:
    LoggerImpl();

    bool init(const LogConfig& conf);

    std::string configToString(void);

private:
    std::mutex mMutex;
    LogConfig mLogConfig;
};


bool LoggerImpl::init(const LogConfig& conf)
{
    return true;
}

std::string LoggerImpl::configToString(void)
{
    std::ostringstream oss;
    oss << "log level:[" << mLogConfig.level << "], "
        << "log mode:[" << mLogConfig.mode << "], "
        << "log path:[" << mLogConfig.file_path << "], "
        << "log size:[" << mLogConfig.file_size << "], "
        << "log backup:[" << mLogConfig.file_backup<< "]"
        << std::endl;

    return oss.str();
}
