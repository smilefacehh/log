#include "StdoutLogger.h"

#include <iostream>

#include "../LogMessage.h"

namespace Nlog
{
StdoutLogger::StdoutLogger() : Logger("stdout") {}

void StdoutLogger::Write(const LogMessage& log_message)
{
    static const char* color_end_tag = "\033[0m";
    const char* color_begin_tag = GetLogColorBySeverity(log_message.GetLogSeverity());
    std::lock_guard<std::mutex> lock_guard(write_mutex_);
    if (color_begin_tag != nullptr)
    {
        std::cout << color_begin_tag << log_message.GetLogHeader(header_pattern_) << color_end_tag
                  << log_message.GetLogText();
    }
    else
    {
        std::cout << log_message.GetLogHeader(header_pattern_) << log_message.GetLogText();
    }
    std::cout.flush();
}

void StdoutLogger::Flush()
{
    std::lock_guard<std::mutex> lock_guard(write_mutex_);
    std::cout.flush();
}
}