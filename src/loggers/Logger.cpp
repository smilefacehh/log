#include "Logger.h"
#include "../LogMessage.h"

namespace Nlog
{
// 默认头部格式
const std::string Logger::DEFAULT_PATTERN = "[%Y-%M-%D %h:%m:%s.%i][%V][%T][%F:%L][%U]";

// 头部格式包含的全部字符
const std::string HEADER_PATTERN_SET{"YMDhmsiVvTFLU"};

bool Logger::SetHeaderPattern(const std::string& header_pattern)
{
    if (IsHeaderPatternValid(header_pattern))
    {
        header_pattern_ = header_pattern;
        return true;
    }
    return false;
}

bool Logger::IsHeaderPatternValid(const std::string& header_pattern)
{
    size_t sz = header_pattern.size();
    if (sz == 0)
    {
        return false;
    }
    for (size_t i = 0; i < sz; ++i)
    {
        if (header_pattern[i] == '%')
        {
            ++i;
            if (i > sz || HEADER_PATTERN_SET.find(header_pattern[i]) == std::string::npos)
            {
                return false;
            }
        }
    }
    return true;
}
}