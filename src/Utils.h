#pragma once

#include <string>

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
}