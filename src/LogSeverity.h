#pragma once

#include <string>

namespace Nlog
{
// 日志等级
// 冗长级别(VERBOSE): 一般用于基础库/框架实现时更加详细的日志，生产环境默认关闭
// 调试级别(DEBUG): 开发时调试程序的正确性输出的调试日志，生产环境默认关闭
// 信息级别(INFO):  用于运行时统计信息输出，如当前程序的状态
// 警告级别(WARN):  程序出现错误，但是可以降级继续往下执行
// 错误级别(ERROR): 程序出现错误，无法继续执行当前逻辑，返回错误
// 致命级别(FATAL): 程序出现致命错误，无法继续运行，应该马上终止程序
enum LogSeverity
{
    VERBOSE,  // 冗长级别
    DEBUG,    // 调试级别
    INFO,     // 信息级别
    WARN,     // 警告级别
    ERROR,    // 错误级别
    FATAL,    // 致命级别
};

/**
 * 获取日志级别对应的名称
 * @param log_severity 日志级别
 * @return 对应日志级别的名称，如果日志级别不合法，返回"UNKNOWN"
 */
const char* GetLogSeverityName(LogSeverity log_severity);

/**
 * 获取日志级别对应的名称缩写
 * @param log_severity 日志级别
 * @return 对应日志级别的名称缩写，如果日志级别不合法，返回'U'
 */
const char GetLogSeverityAbbName(LogSeverity log_severity);

/**
 * 通过名字获取日志级别
 * @param name 日志级别名字
 * @param log_severity 日志级别
 * @return 获取成功返回true，其他返回false
 */
bool GetLogSeverityByName(const std::string& name, LogSeverity& log_severity);

/**
 * 通过日志级别获取日志颜色
 * @param log_severity 日志级别
 * @return 返回对应的颜色，如果没有对应级别的颜色，返回nullptr
 */
const char* GetLogColorBySeverity(LogSeverity log_severity);
}