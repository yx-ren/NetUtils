#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <string>
#include <memory>

enum LOG_LEVEL
{
    LOG_LEVEL_FAILED = 0,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARN,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_TRACE,
    LOG_LEVEL_OFF
};

enum LOG_MODE
{
    LOG_MODE_FILE = 1 << 0,
    LOG_MODE_CONSOLE = 1 << 1,
};

struct LogConfig
{
    LogConfig()
        : level(LOG_LEVEL_TRACE)
        , mode(LOG_MODE_FILE | LOG_MODE_CONSOLE)
        , file_path("EchoServer.log")
        , file_size(1024 * 1024 * 10)
        , file_backup(10)
    {}

    LOG_LEVEL level;
    int mode;
    std::string file_path;
    size_t file_size;
    size_t file_backup;
};

class LoggerImpl;
class Logger
{
public:
    Logger();

    bool init(const LogConfig& conf);

    std::string configToString(void);

private:
    //LoggerImpl* mImpl;
    std::shared_ptr<LoggerImpl> mImpl;
};

#endif
