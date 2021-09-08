#include <sstream>
#include <tinyutils/path_handling.hpp>

namespace tiny {
namespace utils {

std::string GetFilename(const std::string &filepath) {
    return Split(filepath, '/').back();
}

std::string GetFoldername(const std::string &filepath) {
    auto _pathParts = Split(filepath, '/');
    if (_pathParts.size() < 2) {
        return "./";
    }

    return _pathParts[_pathParts.size() - 2];
}

std::string GetFolderpath(const std::string &filepath) {
    auto _pathParts = Split(filepath, '/');
    if (_pathParts.size() < 2) {
        return "./";
    }

    std::stringstream _ss;
    for (size_t i = 0; i < _pathParts.size() - 1; ++i)
        _ss << _pathParts[i] << "/";

    return _ss.str();
}

std::string GetFilenameNoExtension(const std::string &filepath) {
    return Split(Split(filepath, '/').back(), '.').front();
}

}  // namespace utils
}  // namespace tiny
