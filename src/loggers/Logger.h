#pragma once

#include <string>
#include <memory>

#include "../LogMessage.h"

namespace Nlog
{
// Logger代表日志输出后端，用户可以从Logger派生实现自定义的日志输出后端
// 使用方会默认Logger::Write和Logger::Flush接口是线程安全的
class Logger
{
  public:
    explicit Logger(const std::string& name) : name_(name), header_pattern_(DEFAULT_PATTERN) {}

    Logger(const std::string& name, const std::string& header_pattern) : name_(name), header_pattern_(header_pattern) {}

    virtual ~Logger() = default;

    /**
     * 输出日志文本到后端
     * @param log_message 日志消息
     */
    virtual void Write(const LogMessage& log_message) = 0;
    /**
     * 刷新输出缓冲区
     */
    virtual void Flush() = 0;
    /**
     * 获取日志输出名称
     * @return 名称
     */
    const std::string& GetName() const { return name_; }

    /**
     * 设置日志头部格式串
     * @param header_pattern 日志头部格式
     * @return 是否设置成功
     */
    bool SetHeaderPattern(const std::string& header_pattern);

    /**
     * 获取日志头部格式串
     * @return 日志头部格式字符串
     */
    const std::string& GetHeaderPattern() const { return header_pattern_; }

  protected:
    /**
     * 检查日志头部格式是否正确，支持以下格式:
     * %Y 年
     * %M 月
     * %D 日
     * %h 小时
     * %m 分钟
     * %s 秒
     * %i 毫秒
     * %V 日志等级全称
     * %v 日志等级缩写，一个字符
     * %T 线程号
     * %F 文件名
     * %L 行号
     * %U 调用函数名
     * 这些占位符可以任意搭配组合，例子如下：
     * 1.[%Y-%M-%D %h-%m-%s.%i][%V][%T][%F:%L][%U]
     * 2.[%h-%m-%s][%V][%F:%L]
     *
     * @param header_pattern 日志头部格式
     * @return 是否合法格式
     */
    bool IsHeaderPatternValid(const std::string& header_pattern);

  protected:
    // Logger名字
    const std::string name_;

    // Logger头部格式
    std::string header_pattern_;

    // 默认头部格式
    static const std::string DEFAULT_PATTERN;
};
typedef std::shared_ptr<Logger> LoggerPtr;
}