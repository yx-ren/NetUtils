#include <logger/Logger.h>

void Logger::initialize()
{
    using namespace log4cxx;
    using namespace log4cxx::helpers;

    Properties props;

    props.put("log4j.logger.SysLog", "INFO, console");
    props.put("log4j.appender.console", "org.apache.log4j.ConsoleAppender");
    props.put("log4j.appender.console.layout", "org.apache.log4j.PatternLayout");
    props.put("log4j.appender.console.layout.ConversionPattern", "%d [%t] %-5p %.32c - %m%n");

    PropertyConfigurator::configure(props);

    return true;
}

void Logger::initialize(const LoggerParameter& log_param)
{
    using namespace log4cxx;
    using namespace log4cxx::helpers;

    Properties props;

    std::string log_pattern = "%d [%t] <";
    log_pattern += log_info.module_tag;
    log_pattern += ">: %-5p %.32c - %m%n";

    std::string syslog_pattern = "<";
    syslog_pattern += log_info.module_tag;
    syslog_pattern += ">: %-5p %.32c - %m%n";

    std::string sys_value = log_info.level;
    std::string cfg_value = log_info.level;
    std::map<std::string, int> syslogLevelMap;
    syslogLevelMap["TRACE"] = 0;
    syslogLevelMap["DEBUG"] = 1;
    syslogLevelMap["INFO"] = 2;
    syslogLevelMap["WARN"] = 3;
    syslogLevelMap["ERROR"] = 4;
    syslogLevelMap["FATAL"] = 5;

    if (log_info.console_flag)
    {
        sys_value += ", console";
        props.put("log4j.appender.console", "org.apache.log4j.ConsoleAppender");
        props.put("log4j.appender.console.layout", "org.apache.log4j.PatternLayout");
        props.put("log4j.appender.console.layout.ConversionPattern", log_pattern.c_str());
    }
    if (log_info.file_flag)
    {
        sys_value += ", file";
        props.put("log4j.appender.file", "org.apache.log4j.RollingFileAppender");
        props.put("log4j.appender.file.layout", "org.apache.log4j.Patternlayout");
        props.put("log4j.appender.file.layout.ConversionPattern", log_pattern.c_str());
        props.put("log4j.appender.file.File", log_info.file_path.c_str());
        props.put("log4j.appender.file.MaxFileSize", log_info.file_size.c_str());
        props.put("log4j.appender.file.MaxBackupIndex", log_info.file_backup.c_str());
    }
    if (log_info.syslog_flag)
    {
        sys_value += ", syslog";
        props.put("log4j.appender.syslog", "org.apache.log4j.net.SyslogAppender");
        props.put("log4j.appender.syslog.SyslogHost", log_info.syslog_ip.c_str());
        props.put("log4j.appender.syslog.layout", "org.apache.log4j.Patternlayout");
        props.put("log4j.appender.syslog.layout.ConversionPattern", syslog_pattern.c_str());
        props.put("log4j.appender.syslog.Facility", "LOCAL0");
        if (syslogLevelMap[log_info.level] < 2)
            props.put("log4j.appender.syslog.Threshold", "INFO");
        else
            props.put("log4j.appender.syslog.Threshold", log_info.level.c_str());
    }
    props.put("log4j.logger.SysLog", sys_value.c_str());
    props.put("log4j.logger.CfgLog", sys_value.c_str());

    PropertyConfigurator::configure(props);

    return true;
}

void Logger::initialize(log4cxx::helpers::Properties &props)
{
    using namespace log4cxx;
    using namespace log4cxx::helpers;

    PropertyConfigurator::configure(props);

    return true;
}

