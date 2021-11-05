#include "Utils.h"

#include <sys/stat.h>
#include <cstdlib>

namespace Nlog
{
bool DirectoryExists(const std::string& path)
{
    struct stat st = {0};
    if (stat(path.c_str(), &st) == 0)
    {
        return S_ISDIR(st.st_mode);
    }
    return false;
}

bool CreateDirectory(const std::string& path)
{
    std::string mkdir_cmd = "mkdir -p " + path;
    return 0 == system(mkdir_cmd.c_str());
}
}