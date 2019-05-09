#pragma once

#include <string>
#include <vector>

namespace Sentinel {
namespace Utils {

class FileUtils {
public:
    FileUtils() = delete;

    static bool FileExists(const std::string& path);
    static bool DirExists(const std::string& path);
    static int CreateDir(const std::string& path);
    static std::vector<std::string> ListFiles(const std::string& path);
    static std::string GetAbsolutePath(const std::string& path);
};

}  // namespace Utils
}  // namespace Sentinel
