#include "RotateFileLogger.h"

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdlib>
#include <sstream>
#include <iostream>

#include "../Utils.h"

namespace Nlog
{
constexpr char kLoggingFileSuffix[] = "logging";
constexpr char kLoggedFileSuffix[] = "log";
// 单个日志文件最多10M
constexpr int64_t kLogFileSizeLimit = 10 * 1024 * 1024;
// 日志文件切割时间点 整15分钟切割
constexpr int kLogFileJumpTimeLimit = 15;

static size_t GetFileSize(const std::string &file_name) {
    struct stat st;
    if (stat(file_name.c_str(), &st) == 0) {
        return st.st_size;
    }
    // 默认没超过限制，包括文件不存在等情况
    return 0;
}

std::string RotateFileLogger::GetLoggingFileName() {
    return GetFileNameWithTs(last_log_timestamp_, true, log_index_);
}

static bool EndsWith(const std::string &str, const std::string &suffix) {
    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

static time_t GetTsFromLoggingFileName(const std::string &log_file_name,
                                       int &log_index) {
    tm ltm = {0};
    int ret = sscanf(log_file_name.c_str(), "%04d%02d%02d%02d%02d.%d.logging",
                     &ltm.tm_year, &ltm.tm_mon, &ltm.tm_mday, &ltm.tm_hour,
                     &ltm.tm_min, &log_index);
    constexpr int kLogFieldNum = 6;
    if (ret != kLogFieldNum) {
        // format error
        std::cerr << "WARN: log file name format error, log_file_name:"
                         << log_file_name << std::endl;
        return 0;
    }
    ltm.tm_year -= 1900;
    ltm.tm_mon -= 1;
    return mktime(&ltm);
}

// 恢复打印的日志
void RotateFileLogger::RecoverLoggingFile() {
    DIR *dir = opendir(dir_name_.c_str());
    if (nullptr == dir) {
        std::cerr << "INFO: dir not exist, dir:" << dir_name_ << std::endl;
        return;
    }
    struct dirent *dp = nullptr;
    while ((dp = readdir(dir)) != nullptr) {
        if (EndsWith(dp->d_name, kLoggingFileSuffix)) {
            time_t logging_ts = GetTsFromLoggingFileName(dp->d_name, log_index_);
            if (0 == logging_ts) {
                continue;
            }

            if (log_file_ != nullptr) {
                fclose(log_file_);
            }

            std::string log_file_name =
                GetFileNameWithTs(logging_ts, true, log_index_);
            log_file_ = fopen(log_file_name.c_str(), "a");
            if (nullptr == log_file_) {
                std::cerr << "open old log file fail, log_file:"
                                 << log_file_name << std::endl;
            } else {
                written_bytes_ = GetFileSize(log_file_name);
                last_log_timestamp_ = logging_ts;
            }
            break;
        }
    }

    (void)closedir(dir);
}

std::string RotateFileLogger::GetFileNameWithTs(time_t ts, bool is_logging,
                                                int log_index) const {
    // tm_year 从1990开始算
    // tm_mon 从0开始
    tm ltm = {0};
    localtime_r(&ts, &ltm);
    char buff[1024];
    int buff_writen_size = snprintf(
        buff, sizeof buff, "%s/%04d%02d%02d%02d%02d.%d.%s", dir_name_.c_str(),
        ltm.tm_year + 1900, ltm.tm_mon + 1, ltm.tm_mday, ltm.tm_hour, ltm.tm_min,
        log_index, (is_logging ? kLoggingFileSuffix : kLoggedFileSuffix));
    return std::string(buff, buff_writen_size);
}

RotateFileLogger::RotateFileLogger(const std::string &dir_name)
    : Logger("rotate_file"), dir_name_(dir_name), log_file_(nullptr),
      last_log_timestamp_(0), flush_after_write_(false), log_index_(0),
      written_bytes_(0) {}

RotateFileLogger::~RotateFileLogger() {
    if (log_file_ != nullptr) {
        fclose(log_file_);
        log_file_ = nullptr;
    }
}

std::shared_ptr<RotateFileLogger>
RotateFileLogger::Create(const std::string &dir_name) {
    RotateFileLoggerPtr logger(new RotateFileLogger(dir_name));
    if (!logger->Init()) {
        return nullptr;
    }
    return logger;
}

bool RotateFileLogger::Init() {
    if (!DirectoryExists(dir_name_)) {
        // 目录不存在且创建目录失败，则失败
        return CreateDirectory(dir_name_);
    } else {
        RecoverLoggingFile();
    }
    return true;
}

void RotateFileLogger::CheckFileAndRotate() {
    int64_t cur_time = time(0);

    bool need_rotate = false;
    int new_log_index = log_index_;
    if (last_log_timestamp_ > 0) {
        if (IsTimeJump(cur_time, last_log_timestamp_)) {
            need_rotate = true;
            // 如果是切到下一时间点的日志，则重置log_index
            new_log_index = 0;
        } else if (written_bytes_ > kLogFileSizeLimit) {
            need_rotate = true;
            ++new_log_index;
        }
    } else if (0 == last_log_timestamp_) {
        need_rotate = true;
        new_log_index = 0;
    }

    if (!need_rotate) {
        return;
    }

    // 切分日志
    if (log_file_ != nullptr) {
        // 超过1h，logging后缀文件更名为log后缀
        if (last_log_timestamp_ != 0) {
            std::string last_logging_suffix = GetLoggingFileName();
            std::string last_logged_suffix =
                GetFileNameWithTs(last_log_timestamp_, false, log_index_);
            // NOTE ignore rename fail
            rename(last_logging_suffix.c_str(), last_logged_suffix.c_str());
        }
        fclose(log_file_);
        log_file_ = nullptr;
    }
    written_bytes_ = 0;

    // 打开新的日志文件
    std::string log_file_name = GetFileNameWithTs(cur_time, true, new_log_index);

    log_file_ = fopen(log_file_name.c_str(), "a");
    if (nullptr == log_file_) {
        // 打开文件失败直接返回，不更新最新的时间戳
        return;
    }
    last_log_timestamp_ = cur_time;
    log_index_ = new_log_index;
}

bool RotateFileLogger::IsTimeJump(time_t cur_ts, time_t logging_ts) {
    // NOTICE localtime线程不安全，使用localtime_r
    // 每 15 分钟做一次切分
    tm cur_tm = {0};
    tm logging_tm = {0};
    localtime_r(&cur_ts, &cur_tm);
    localtime_r(&logging_ts, &logging_tm);
    return ((0 == cur_tm.tm_min % kLogFileJumpTimeLimit &&
             cur_tm.tm_min != logging_tm.tm_min) ||
            cur_tm.tm_hour != logging_tm.tm_hour ||
            cur_tm.tm_mday != logging_tm.tm_mday ||
            cur_tm.tm_mon != logging_tm.tm_mon ||
            cur_tm.tm_year != logging_tm.tm_year);
}

void RotateFileLogger::Write(const LogMessage &log_message) {
    std::lock_guard<std::mutex> lock_guard(write_mutex_);

    // 检查是否需要日志切分
    CheckFileAndRotate();

    if (log_file_ != nullptr) {
        std::stringstream ss;
        ss << log_message.GetLogHeader(header_pattern_) << log_message.GetLogText();
        const std::string &log_text(ss.str());
        written_bytes_ += log_text.size();
        fwrite(log_text.c_str(), log_text.size(), 1, log_file_);
        if (flush_after_write_)
            fflush(log_file_);
    }
}

void RotateFileLogger::Flush() {
    std::lock_guard<std::mutex> lock_guard(write_mutex_);
    if (log_file_) {
        fflush(log_file_);
    }
}

void RotateFileLogger::SetFlushAfterWrite(bool on) {
    std::lock_guard<std::mutex> lock_guard(write_mutex_);
    flush_after_write_ = on;
}

}