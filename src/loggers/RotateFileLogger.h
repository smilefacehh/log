#pragma once

#include <cstdio>
#include <ctime>
#include <memory>
#include <mutex>

#include "Logger.h"

namespace Nlog
{
class RotateFileLogger : public Logger
{
  public:
    ~RotateFileLogger() override;

    void Write(const LogMessage& log_message) override;
    void Flush() override;

    static std::shared_ptr<RotateFileLogger> Create(
        const std::string& dir_name);

    /**
     * 设置是否write后自动调用flush
     * @param on true代表自动flush
     */
    void SetFlushAfterWrite(bool on);

  private:
    explicit RotateFileLogger(const std::string& dir_name);

    bool Init();

    /**
     * @brief 获取一个时间戳对应的文件名
     * @param ts 时间戳
     * @param is_logging 是否正在日志
     * @param log_index 日志文件序号
     * @return 对应的文件名
     **/
    std::string GetFileNameWithTs(time_t ts, bool is_logging,
                                  int log_index) const;

    /**
     * @brief 获取当前正在日志的文件名
     * @return
     **/
    std::string GetLoggingFileName();

    /**
     * @brief 恢复正在打日志的文件（用于重启恢复）
     **/
    void RecoverLoggingFile();

    /**
     * @brief 检查并切分日志
     **/
    void CheckFileAndRotate();

    static bool IsTimeJump(time_t cur_ts, time_t logging_ts);

  private:
    // write_mutex_ 用于同步Write函数
    std::mutex write_mutex_;
    // 日志文件输出目录
    const std::string dir_name_;
    // 当前输出日志文件的句柄
    FILE* log_file_;
    // 最后一次打印日志的时间戳
    time_t last_log_timestamp_;
    // write后自动调用flush
    bool flush_after_write_;
    // 当前时间节点下输出日志文件的序号
    int log_index_;
    // 单日志文件已写入的字节数
    size_t written_bytes_;
};

typedef std::shared_ptr<RotateFileLogger> RotateFileLoggerPtr;

}
