#pragma once

namespace Nlog {
// 日志级别
enum Level
{
    VERBOSE = 0,
    DEBUG = 1,
    INFO = 2,
    WARN = 3,
    ERROR = 4,
    FATAL = 5,
    OFF = 6,
    N_LEVELS
};

#ifndef LEVEL_NAMES
#define LEVEL_NAMES                                                                                                    \
    {                                                                                                                  \
        "VERBOSE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL", "OFF"                                                    \
    }
#endif

#ifndef SHORT_LEVEL_NAMES
#define SHORT_LEVEL_NAMES                                                                                              \
    {                                                                                                                  \
        "V", "D", "I", "W", "E", "F", "O"                                                                              \
    }
#endif

#ifndef COLOR_TAGS
#define COLOR_TAGS                                                                                                     \
    {                                                                                                                  \
        "\033[35m", "\033[36m", "\033[32m", "\033[33m", "\033[31m", "\033[31m", "\033[0m"                              \
    }
#endif

// C++14
#if __cplusplus >= 201402L
using std::make_unique;
#else
template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
    static_assert(!std::is_array<T>::value, "arrays not supported");
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
#endif

// Level枚举转名字
const char* ToNameStr(Level level);

// Level枚举转名字缩写
const char* ToShortNameStr(Level level);

// 名字转Level枚举
Level FromNameStr(const char* name);

// Level枚举获取对应颜色标签
const char* ColorTag(Level level);
}  // namespace Nlog