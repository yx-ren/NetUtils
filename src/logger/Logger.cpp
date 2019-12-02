#include "Logger.h"

#include <time.h>
#include <fstream>
#include <ostream>
#include <sstream>
#include <iostream>
#include <mutex>
#include <chrono>
#include <iomanip>
#include <thread>
#include <iterator>

class FileHelper
{
public:
    FileHelper() = delete;
    FileHelper(const FileHelper&) = delete;
    FileHelper& operator=(const FileHelper&) = delete;

    static bool getFileSize(std::string& path, size_t& fsize);
    static bool getFileName(std::string& path);
    static bool getFilePathPrefix(std::string& path);
    static bool moveFile(std::string& oldPath, std::string& newPath);
    static bool removeFile(std::string& path);
};

bool FileHelper::getFileSize(std::string& path, size_t& fsize)
{
    std::ifstream ifs(path, std::ios::in | std::ios::binary);
    if (!ifs.is_open())
        return false;

    std::string file_buf((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    fsize = file_buf.size();

    return true;
}

bool FileHelper::getFileName(std::string& path)
{
    return true;
}

bool FileHelper::getFilePathPrefix(std::string& path)
{
    return true;
}

bool FileHelper::moveFile(std::string& oldPath, std::string& newPath)
{
    return true;
}

bool FileHelper::removeFile(std::string& path)
{
    return true;
}

std::string LoggerHelper::levelToString(LOG_LEVEL level)
{
    std::string log_level("");
    switch(level)
    {
        case LOG_LEVEL::LOG_LEVEL_OFF:
            log_level = "OFF";
            break;
        case LOG_LEVEL::LOG_LEVEL_FAILED:
            log_level = "FAILED";
            break;
        case LOG_LEVEL_ERROR:
            log_level = "ERROR";
            break;
        case LOG_LEVEL_WARN:
            log_level = "WARN";
            break;
        case LOG_LEVEL_INFO:
            log_level = "INFO";
            break;
        case LOG_LEVEL_DEBUG:
            log_level = "DEBUG";
            break;
        case LOG_LEVEL_TRACE:
            log_level = "TRACE";
            break;
        default:
            log_level = "UNKNOWN";
            break;
    }

    return log_level;
}

std::string LoggerHelper::modeToString(int mode)
{
    std::string log_mode;
    if (mode & LOG_MODE_FILE)
        log_mode += "FILE; ";
    if (mode & LOG_MODE_CONSOLE)
        log_mode += "CONSOLE; ";

    return log_mode;
}

std::string LoggerHelper::configToString(LogConfig conf)
{
    std::ostringstream oss;
    oss << "log level:[" << levelToString(conf.level) << "], "
        << "log mode:[" << modeToString(conf.mode) << "], "
        << "log path:[" << conf.file_path << "], "
        << "log size:[" << conf.file_size << "] bytes, "
        << "log backup:[" << conf.file_backup<< "]"
        << std::endl;

    return oss.str();
}

// ---------------------------------------- //
// LoggerImpl
// ---------------------------------------- //

class LoggerImpl
{
public:
    explicit LoggerImpl(const LogConfig& conf)
        : mLogConfig(conf)
        , mOfs(NULL)
    {}

    virtual ~LoggerImpl()
    {
        if (mOfs)
            mOfs->close();
    }

    bool init(void);

    void setConfig(const LogConfig& conf);
    LogConfig getConfig(void);

    void writeLog(std::string& log, LOG_LEVEL level);
    void writeLog(std::string& tag, std::string& log, LOG_LEVEL level);

protected:
    virtual inline std::string generatePrefix(void);

    virtual inline std::string generateTimestamp(void);
    virtual inline std::string generateThreadID(void);
    virtual inline std::string generateModuleTag(void);
    virtual inline std::string generateLogLevel(void);
    virtual inline std::string generateLogTag(void);
    virtual inline std::string generateClassTag(void);
    virtual inline std::string generateFunctionTag(void);

private:
    std::mutex mMutex;
    LogConfig mLogConfig;
    std::shared_ptr<std::ofstream> mOfs;
};

bool LoggerImpl::init(void)
{
    mOfs.reset(new std::ofstream(mLogConfig.file_path, std::ios::out | std::ios::app));
    if (!mOfs->is_open())
    {
        std::cerr << "call std::ofstream() failed, "
            << "log file path:[" << mLogConfig.file_path << "]" << std::endl;
        return false;
    }

    return true;
}

void LoggerImpl::setConfig(const LogConfig& conf)
{
    mLogConfig = conf;
}

LogConfig LoggerImpl::getConfig(void)
{
    return mLogConfig;
}

void LoggerImpl::writeLog(std::string& log, LOG_LEVEL level)
{
    if (level > mLogConfig.level)
        return;

    if (mOfs == NULL)
    {
        std::cerr << "logger not be inited, call init() and try again" << std::endl;
        return;
    }

    std::ostringstream oss;
    std::string log_prefix = generatePrefix();
    oss << log_prefix << log << std::endl;

    std::lock_guard<std::mutex> lk(mMutex);
    if (mLogConfig.mode & LOG_MODE_FILE)
    {
        *mOfs << oss.str();
    }

    if (mLogConfig.mode & LOG_MODE_CONSOLE)
    {
        std::cout << oss.str();
    }

    return;
}

void LoggerImpl::writeLog(std::string& tag, std::string& log, LOG_LEVEL level)
{
    return;
}

std::string LoggerImpl::generatePrefix(void)
{
    std::ostringstream oss;
    oss << generateTimestamp() << " "
        << "[" << generateThreadID() << "] "
        << "<" << generateModuleTag()<< ">: "
        << generateLogLevel()<< " "
        << generateLogTag() << "." << generateFunctionTag() << " - ";

    return oss.str();
}

std::string LoggerImpl::generateTimestamp(void)
{
    auto time_now    = std::chrono::system_clock::now();

    auto duration_ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(time_now.time_since_epoch());
    auto part_ms     =
        duration_ms - std::chrono::duration_cast<std::chrono::seconds>(duration_ms);

    auto duration_us =
        std::chrono::duration_cast<std::chrono::microseconds>(time_now.time_since_epoch());
    auto part_us     =
        duration_us - std::chrono::duration_cast<std::chrono::milliseconds>(duration_us);

    struct tm local_time;
    time_t ctime = std::chrono::system_clock::to_time_t(time_now);
#if defined(WIN32) || defined(_WIN32)
    _localtime64_s(&local_time, &ctime);
#else
    localtime_r(&ctime, &local_time);
#endif

    char timestamp[0x7f] = {0};
    std::strftime(timestamp, sizeof(timestamp),
        "%Y-%m-%d %H:%M:%S", &local_time);

    std::ostringstream oss;
    oss << timestamp
        << "," << std::setw(3) << std::setfill('0') << part_ms.count()
        << "," << std::setw(3) << std::setfill('0') << part_us.count();

    return oss.str();
}

std::string LoggerImpl::generateThreadID(void)
{
    std::ostringstream oss;
    oss << std::this_thread::get_id();
    unsigned int tid = std::stoul(oss.str());

    oss.str("");
    oss.clear();

    oss << "0x" << std::setw(8) << std::setfill('0')
        << std::hex << tid;

    return oss.str();
}

std::string LoggerImpl::generateModuleTag(void)
{
    return "";
}

std::string LoggerImpl::generateLogLevel(void)
{
    return LoggerHelper::levelToString(mLogConfig.level);
}

std::string LoggerImpl::generateLogTag(void)
{
    return "";
}

std::string LoggerImpl::generateClassTag(void)
{
    return "";
}

std::string LoggerImpl::generateFunctionTag(void)
{
    return "";
}

// ---------------------------------------- //
// Logger
// ---------------------------------------- //

Logger::Logger(const LogConfig& conf)
{
    mImpl.reset(new LoggerImpl(conf));
}

bool Logger::init(void)
{
    return mImpl->init();
}

void Logger::setConfig(const LogConfig& conf)
{
    return mImpl->setConfig(conf);
}

LogConfig Logger::getConfig(void)
{
    return mImpl->getConfig();
}

void Logger::writeLog(std::string& log, LOG_LEVEL level)
{
    return mImpl->writeLog(log, level);
}

void Logger::writeLog(std::string& tag, std::string& log, LOG_LEVEL level)
{
    return mImpl->writeLog(tag, log, level);
}
