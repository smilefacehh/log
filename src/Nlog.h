#pragma once

#include <chrono>
#include <memory>

#include "details/LogMessage.h"
#include "details/LogSeverity.h"
#include "loggers/Logger.h"

// 外部指定打印方法
#define PLUG_LOG_VALID(log_severity) (Nlog::Logging::IsPlugLoggerValid(Nlog::log_severity))
#define PLUG_LOG(log_severity)                                                                                         \
    (Nlog::LogMessage(Nlog::log_severity, __FILE__, __func__, __LINE__, &Nlog::Logging::LogToPlug##log_severity))

// 内部打印方法
#define LOG_IS_ON(log_severity) (Nlog::Logging::IsLogSeverityOn(Nlog::log_severity))
#define LOG(log_severity)                                                                                              \
    (Nlog::LogMessage(Nlog::log_severity, __FILE__, __func__, __LINE__, &Nlog::Logging::LogToAllLoggers))

// 如果外部指定打印方法，则选择外部方法，否则选择内部打印方法
#define LOG_CHOOSE(log_severity)                                                                                       \
    if (LOG_IS_ON(log_severity)) ((PLUG_LOG_VALID(log_severity)) ? (PLUG_LOG(log_severity)) : (LOG(log_severity)))

#define LOG_VERBOSE(module) LOG_CHOOSE(VERBOSE) << "<" #module ">"
#define LOG_DEBUG(module) LOG_CHOOSE(DEBUG) << "<" #module ">"
#define LOG_INFO(module) LOG_CHOOSE(INFO) << "<" #module ">"
#define LOG_WARN(module) LOG_CHOOSE(WARN) << "<" #module ">"
#define LOG_ERROR(module) LOG_CHOOSE(ERROR) << "<" #module ">"
#define LOG_FATAL(module) LOG_CHOOSE(FATAL) << "<" #module ">"

#define LOG_V(value) #value ":" << value

namespace Nlog {
/**
 * Logging是日志管理类，非线程安全。
 */
class Logging
{
  public:
    /**
     * 设置全局日志打印等级
     * @param log_severity 日志等级
     */
    static void SetLogSeverity(LogSeverity log_severity);
    /**
     * 获取全局日志打印级别
     * @return 日志等级
     */
    static LogSeverity GetLogSeverity();
    /**
     * 判断特定的日志等级是否打开
     * @param log_severity 日志等级
     * @return true表示打开，false表示关闭
     */
    static bool IsLogSeverityOn(LogSeverity log_severity);
    /**
     * 增加日志输出后端
     * @param logger
     * @return true表示添加成功，false表示添加失败
     */
    static bool AddLogger(const LoggerPtr& logger);
    /**
     * 通过名字移除日志输出后端。移除成功返回true，否则返回false
     * @param name 日志输出后端名称
     * @return true表示移除成功，false移除失败
     */
    static bool RemoveLogger(const std::string& name);
    /**
     * 输出日志消息到当前所有的日志输出后端
     * @param log_message 日志消息
     */
    static void LogToAllLoggers(const LogMessage& log_message);
    /**
     * 刷新所有日志输出后端的缓冲区(一般用于程序结束前，保证缓冲区内的日志可以正确写到文件）
     */
    static void FlushAllLoggers();
    /**
     * 输出日志到标准输出
     * @param log_message 日志消息
     */
    static void LogToStdout(const LogMessage& log_message);
    /**
     * 输出日志到标准错误输出
     * @param log_message 日志消息
     */
    static void LogToStderr(const LogMessage& log_message);

    /**
     * 定时刷新日志
     * @param interval 间隔时长（s）
     */
    static void FlushEvery(std::chrono::seconds interval);

    /**
     * 关闭时调用
     */
    static void ShutDown();

    typedef std::function<void(const char*)> PlugLogFunc;

    /**
     * 设置外部logger方法，接管日志输出
     * @param log_verbose verbose
     * @param log_debug debug
     * @param log_info info
     * @param log_warn warn
     * @param log_error error
     * @param log_fatal fata
     */
    static void SetPlugLoggerFuncs(PlugLogFunc log_verbose, PlugLogFunc log_debug, PlugLogFunc log_info,
                                   PlugLogFunc log_warn, PlugLogFunc log_error, PlugLogFunc log_fatal);

    /**
     * 指定等级的logger是否有外部接管方法，如果没有则会调用自己的方法
     * @param log_severity 日志等级
     * @return true 有外部接管方法
     */
    static bool IsPlugLoggerValid(LogSeverity log_severity);

    /**
     * 调用外部logger方法
     * @param log_message log消息
     */
    static void LogToPlugVERBOSE(const LogMessage& log_message);
    static void LogToPlugDEBUG(const LogMessage& log_message);
    static void LogToPlugINFO(const LogMessage& log_message);
    static void LogToPlugWARN(const LogMessage& log_message);
    static void LogToPlugERROR(const LogMessage& log_message);
    static void LogToPlugFATAL(const LogMessage& log_message);
};
}  // namespace Nlog