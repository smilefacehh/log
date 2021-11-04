#include "LogSeverity.h"

namespace Nlog
{
constexpr int NUM_LOG_SEVERITYS = 6;
const char* log_severity_names[NUM_LOG_SEVERITYS] = {"VERBOSE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"};
const char log_serverity_abb_names[NUM_LOG_SEVERITYS] = {'V', 'D', 'I', 'W', 'E', 'F'};

const char* GetLogSeverityName(LogSeverity log_severity)
{
    if (log_severity >= 0 && log_severity < NUM_LOG_SEVERITYS)
    {
        return log_severity_names[log_severity];
    }
    return "UNKNOWN";
}

const char GetLogSeverityAbbName(LogSeverity log_severity)
{
    if (log_severity >= 0 && log_severity < NUM_LOG_SEVERITYS)
    {
        return log_serverity_abb_names[log_severity];
    }
    return 'U';
}

bool GetLogSeverityByName(const std::string& name, LogSeverity& log_severity)
{
    for (int i = 0; i < NUM_LOG_SEVERITYS; i++)
    {
        if (log_severity_names[i] == name)
        {
            log_severity = static_cast<LogSeverity>(i);
            return true;
        }
    }
    return false;
}

const char* GetLogColorBySeverity(LogSeverity log_severity)
{
    switch (log_severity)
    {
        case VERBOSE:
            return "\033[36m";
        case DEBUG:
            return "\033[35m";
        case INFO:
            return "\033[34m";
        case WARN:
            return "\033[33m";
        case ERROR:
            return "\033[31m";
        case FATAL:
            return "\033[31m";
    }
    return nullptr;
}
}