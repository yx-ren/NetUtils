#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <string>
#include <memory>

struct LoggerParameter
{
public:
    LoggerParameter()
    {
        module_tag = "echo";
        level = "TRACE";
        console_flag = true;
        file_flag = true;
        file_path = "/var/log/echo_server";
        file_size = "1024000";
        file_backup = "10";
        syslog_flag = true;
        syslog_ip = "127.0.0.1";
    }
    std::string module_tag;
    std::string level;
    bool console_flag;
    bool file_flag;
    std::string file_path;
    std::string file_size;
    std::string file_backup;
    bool syslog_flag;
    std::string syslog_ip;
};

class Logger
{
public:
    void initialize();
    void initialize(const LoggerParameter& log_param);
    void initialize(const log4cxx::helpers::Properties& props);

private:
    Logger(const Logger&);
    Logger& operator=(const Logger&);
};

#endif
