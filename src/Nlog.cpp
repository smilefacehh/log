#include "Nlog.h"
#include <iostream>
#include <vector>

namespace Nlog
{
// 打印日志等级
static LogSeverity g_log_severity = DEBUG;

// 日志输出后端
static std::vector<LoggerPtr> g_loggers;

void Logging::SetLogSeverity(LogSeverity log_severity) { g_log_severity = log_severity; }

LogSeverity Logging::GetLogSeverity() { return g_log_severity; }

bool Logging::IsLogSeverityOn(LogSeverity log_severity) { return log_severity >= GetLogSeverity(); }

bool Logging::AddLogger(const LoggerPtr& logger)
{
    if(logger == nullptr) return false;

    for (auto& g_logger : g_loggers)
    {
        if (g_logger->GetName() == logger->GetName())
        {
            return false;
        }
    }

    g_loggers.push_back(logger);
    return true;
}

bool Logging::RemoveLogger(const std::string& name)
{
    for (size_t i = 0; i < g_loggers.size(); i++)
    {
        if (g_loggers[i]->GetName() == name)
        {
            // 交换当前的Logger和最后的Logger
            g_loggers[i] = g_loggers.back();
            g_loggers.pop_back();
            return true;
        }
    }
    return false;
}

void Logging::LogToAllLoggers(const LogMessage& log_message)
{
    for (auto& logger : g_loggers)
    {
        logger->Write(log_message);
    }
}

void Logging::FlushAllLoggers()
{
    for (auto& logger : g_loggers)
    {
        logger->Flush();
    }
}

void Logging::LogToStdout(const LogMessage& log_message)
{
    std::cout << log_message.GetLogText();
    std::cout.flush();
}

void Logging::LogToStderr(const LogMessage& log_message)
{
    std::cerr << log_message.GetLogText();
    std::cerr.flush();
}

static void (*PlugLogVERBOSE)(const char* log_text) = nullptr;
static void (*PlugLogDEBUG)(const char* log_text) = nullptr;
static void (*PlugLogINFO)(const char* log_text) = nullptr;
static void (*PlugLogWARN)(const char* log_text) = nullptr;
static void (*PlugLogERROR)(const char* log_text) = nullptr;
static void (*PlugLogFATAL)(const char* log_text) = nullptr;

void Logging::SetPlugLoggerFuncs(void (*log_verbose)(const char* log_text), void (*log_debug)(const char* log_text),
                                 void (*log_info)(const char* log_text), void (*log_warn)(const char* log_text),
                                 void (*log_error)(const char* log_text), void (*log_fatal)(const char* log_text))
{
    PlugLogVERBOSE = log_verbose;
    PlugLogDEBUG = log_debug;
    PlugLogINFO = log_info;
    PlugLogWARN = log_warn;
    PlugLogERROR = log_error;
    PlugLogFATAL = log_fatal;
}

bool Logging::IsPlugLoggerValid(LogSeverity log_severity)
{
    if (log_severity == LogSeverity::VERBOSE)
    {
        return PlugLogVERBOSE != nullptr;
    }
    else if (log_severity == LogSeverity::DEBUG)
    {
        return PlugLogDEBUG != nullptr;
    }
    else if (log_severity == LogSeverity::INFO)
    {
        return PlugLogINFO != nullptr;
    }
    else if (log_severity == LogSeverity::WARN)
    {
        return PlugLogWARN != nullptr;
    }
    else if (log_severity == LogSeverity::ERROR)
    {
        return PlugLogERROR != nullptr;
    }
    else if (log_severity == LogSeverity::FATAL)
    {
        return PlugLogFATAL != nullptr;
    }
    return false;
}

void Logging::LogToPlugVERBOSE(const LogMessage& log_message) { PlugLogVERBOSE(log_message.GetLogText()); }
void Logging::LogToPlugDEBUG(const LogMessage& log_message) { PlugLogDEBUG(log_message.GetLogText()); }
void Logging::LogToPlugINFO(const LogMessage& log_message) { PlugLogINFO(log_message.GetLogText()); }
void Logging::LogToPlugWARN(const LogMessage& log_message) { PlugLogWARN(log_message.GetLogText()); }
void Logging::LogToPlugERROR(const LogMessage& log_message) { PlugLogERROR(log_message.GetLogText()); }
void Logging::LogToPlugFATAL(const LogMessage& log_message) { PlugLogFATAL(log_message.GetLogText()); }
}