#pragma once

#include <string>
#include <memory>
#include <type_traits>

namespace Nlog
{
/**
 * 判断指定的目录是否存在
 * @param path 目录路径
 * @return true表示目录路径存在，否则不存在
 */
bool DirectoryExists(const std::string& path);

/**
 * 创建指定的目录
 * @param path 目录路径
 * @return true表示创建成功，否则失败
 */
bool CreateDirectory(const std::string& path);


#if __cplusplus >= 201402L
using std::make_unique;
#else
template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args &&...args)
{
    static_assert(!std::is_array<T>::value, "arrays not supported");
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
#endif
}