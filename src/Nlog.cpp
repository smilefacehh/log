#include "Nlog.h"

#include <iostream>
#include <vector>

#include "details/PeriodicWorker.h"
#include "details/Utils.h"

namespace Nlog {
// 打印日志等级
static LogSeverity g_log_severity = DEBUG;

// 日志输出后端
static std::vector<LoggerPtr> g_loggers;

// 定时刷新日志
static std::unique_ptr<PeriodicWorker> periodic_flusher_;

void Logging::SetLogSeverity(LogSeverity log_severity) { g_log_severity = log_severity; }

LogSeverity Logging::GetLogSeverity() { return g_log_severity; }

bool Logging::IsLogSeverityOn(LogSeverity log_severity) { return log_severity >= GetLogSeverity(); }

bool Logging::AddLogger(const LoggerPtr& logger)
{
    if (logger == nullptr) return false;

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

void Logging::FlushEvery(std::chrono::seconds interval)
{
    periodic_flusher_ = make_unique<PeriodicWorker>(&Logging::FlushAllLoggers, interval);
}

void Logging::ShutDown() { periodic_flusher_.reset(); }

static Logging::PlugLogFunc plug_log_verbose = nullptr;
static Logging::PlugLogFunc plug_log_info = nullptr;
static Logging::PlugLogFunc plug_log_debug = nullptr;
static Logging::PlugLogFunc plug_log_warn = nullptr;
static Logging::PlugLogFunc plug_log_error = nullptr;
static Logging::PlugLogFunc plug_log_fatal = nullptr;

void Logging::SetPlugLoggerFuncs(PlugLogFunc log_verbose, PlugLogFunc log_debug, PlugLogFunc log_info,
                                 PlugLogFunc log_warn, PlugLogFunc log_error, PlugLogFunc log_fatal)
{
    plug_log_verbose = log_verbose;
    plug_log_debug = log_debug;
    plug_log_info = log_info;
    plug_log_warn = log_warn;
    plug_log_error = log_error;
    plug_log_fatal = log_fatal;
}

bool Logging::IsPlugLoggerValid(LogSeverity log_severity)
{
    if (log_severity == LogSeverity::VERBOSE)
    {
        return plug_log_verbose != nullptr;
    }
    else if (log_severity == LogSeverity::DEBUG)
    {
        return plug_log_debug != nullptr;
    }
    else if (log_severity == LogSeverity::INFO)
    {
        return plug_log_info != nullptr;
    }
    else if (log_severity == LogSeverity::WARN)
    {
        return plug_log_warn != nullptr;
    }
    else if (log_severity == LogSeverity::ERROR)
    {
        return plug_log_error != nullptr;
    }
    else if (log_severity == LogSeverity::FATAL)
    {
        return plug_log_fatal != nullptr;
    }
    return false;
}

void Logging::LogToPlugVERBOSE(const LogMessage& log_message) { plug_log_verbose(log_message.GetLogText()); }
void Logging::LogToPlugDEBUG(const LogMessage& log_message) { plug_log_debug(log_message.GetLogText()); }
void Logging::LogToPlugINFO(const LogMessage& log_message) { plug_log_info(log_message.GetLogText()); }
void Logging::LogToPlugWARN(const LogMessage& log_message) { plug_log_warn(log_message.GetLogText()); }
void Logging::LogToPlugERROR(const LogMessage& log_message) { plug_log_error(log_message.GetLogText()); }
void Logging::LogToPlugFATAL(const LogMessage& log_message) { plug_log_fatal(log_message.GetLogText()); }
}  // namespace Nlog