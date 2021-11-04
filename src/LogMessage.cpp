#include "LogMessage.h"

#include <sys/syscall.h>
#include <sys/time.h>
#include <unistd.h>

#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace Nlog
{
LogStreamBuf::LogStreamBuf()
{
    // kBufferSize 至少要大于等于2, 保证有足够的空间预留给后续添加'\n'和'\0'
    static_assert(kBufferSize > 0, "kBufferSize must be large than 2");
    setp(buffer, buffer + kBufferSize);

    buffer[0] = '\0';
}

// 实现std::streambuf的overflow接口，当数据写入并且缓冲区已满时会被调用
LogStreamBuf::int_type LogStreamBuf::overflow(int_type ch)
{
    // 丢弃溢出的字符
    return ch;
}

// 获取缓冲区
const char* LogStreamBuf::GetBuffer() const { return buffer; }

// 获取buffer的已用空间大小
size_t LogStreamBuf::GetCount() const { return pptr() - pbase(); }

// 获取buffer的最后一个字符
// 如果buffer为空，返回EOF;否则返回最后一个字符
char LogStreamBuf::GetBackChar() const
{
    if (GetCount() == 0) return EOF;
    return *(pptr() - 1);
}

LogMessage::LogMessage(LogSeverity msg_severity, const char* const file, const char* const func, int line,
                       const LogFunc& log_func)
    : msg_severity_(msg_severity),
      file_(file),
      func_(func),
      line_(line),
      log_func_(log_func),
      flushed_(false),
      stream_buf_(),
      stream_(&stream_buf_),
      nums_to_log_(0)
{
    if (gettimeofday(&tv_, nullptr) != 0)
    {
        time(&tv_.tv_sec);
        tv_.tv_usec = 0;
    }
    //    FillLogHeader();
}

LogMessage::~LogMessage() { Flush(); }

static inline const char* GetFileName(const char* path)
{
    const char* file = std::strrchr(path, '/');
    if (file) return file + 1;
    return path;
}

inline long GetThreadId()
{
    // 使用tls确保每个线程只会执行一次syscall获取线程ID
    static thread_local long thread_id = syscall(__NR_gettid);
    return thread_id;
}

std::string LogMessage::GetLogHeader(const std::string& pattern) const
{
    size_t sz = pattern.size();
    if (sz == 0)
    {
        return std::string();
    }

    thread_local struct tm result = {};
    (void)localtime_r(&tv_.tv_sec, &result);

    std::stringstream ss;
    for (size_t i = 0; i < sz; ++i)
    {
        if (pattern[i] == '%')
        {
            ++i;
            if (i < sz)
            {
                const char& c = pattern[i];
                if (c == 'Y')
                {
                    ss << std::setw(4) << 1900 + result.tm_year;
                }
                else if (c == 'M')
                {
                    ss << std::setw(2) << std::setfill('0') << 1 + result.tm_mon;
                }
                else if (c == 'D')
                {
                    ss << std::setw(2) << std::setfill('0') << result.tm_mday;
                }
                else if (c == 'h')
                {
                    ss << std::setw(2) << std::setfill('0') << result.tm_hour;
                }
                else if (c == 'm')
                {
                    ss << std::setw(2) << std::setfill('0') << result.tm_min;
                }
                else if (c == 's')
                {
                    ss << std::setw(2) << std::setfill('0') << result.tm_sec;
                }
                else if (c == 'i')
                {
                    ss << std::setw(3) << std::setfill('0') << tv_.tv_usec / 1000;
                }
                else if (c == 'V')
                {
                    ss << std::setw(5) << std::setfill(' ') << GetLogSeverityName(msg_severity_);
                }
                else if (c == 'v')
                {
                    ss << GetLogSeverityAbbName(msg_severity_);
                }
                else if (c == 'T')
                {
                    ss << GetThreadId();
                }
                else if (c == 'F')
                {
                    ss << GetFileName(file_);
                }
                else if (c == 'L')
                {
                    ss << line_;
                }
                else if (c == 'U')
                {
                    ss << func_;
                }
            }
        }
        else
        {
            ss << pattern[i];
        }
    }

    return ss.str();
}

void LogMessage::Flush()
{
    if (flushed_) return;

    nums_to_log_ = stream_buf_.GetCount();
    if (nums_to_log_ == 0) return;

    if (stream_buf_.GetBackChar() != '\n')
    {
        // 已经保证stream_buf_至少还有两个字符的可用空间
        stream_buf_.buffer[nums_to_log_++] = '\n';
    }
    // 确保buffer是一个C风格的字符串，对于某些输出流来说会比较方便使用
    stream_buf_.buffer[nums_to_log_] = '\0';

    log_func_(*this);

    flushed_ = true;
}
}