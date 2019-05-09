#include "sentinel-core/utils/file_utils.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

#include <errno.h>
#include <string.h>

#include <fstream>
#include <iostream>


namespace Sentinel {
namespace Utils {

bool FileUtils::FileExists(const std::string& path) {
    std::ifstream input_file(path);
    return input_file.is_open();
}

bool FileUtils::DirExists(const std::string& path) {
    DIR* const dir = ::opendir(path.c_str());

    bool dir_exists = (nullptr != dir);
    if (dir_exists) {
        ::closedir(dir);
    }

    return dir_exists;
}

int FileUtils::CreateDir(const std::string& path) {
    auto s = path;
    size_t pos=0;
    std::string dir;
    int ret;

    if(s[s.size()-1]!='/'){
        s += '/';
    }

    while((pos = s.find_first_of('/', pos)) != std::string::npos) {
        dir = s.substr(0, pos++);
        if(dir.size() == 0) {
            continue; // if leading / first time is 0 length
        }
        if((ret=mkdir(dir.c_str(), S_IRWXU)) && errno!=EEXIST) {
            return ret;
        }
    }
    return ret;
}

std::vector<std::string> FileUtils::ListFiles(const std::string& path) {
    std::vector<std::string> files;

    auto *dir = opendir (path.c_str());
    if (dir == nullptr) {
        return files;
    }

    struct dirent *ent;
    while ((ent = readdir (dir)) != nullptr) {
        files.push_back(ent->d_name);
    }
    closedir (dir);

    return files;
}


std::string FileUtils::GetAbsolutePath(const std::string& path)
{
    char abs_path[40960]={0};
    realpath(path.c_str(), abs_path);
    return std::string(abs_path);
}

}  // namespace Utils
}  // namespace Sentinel
