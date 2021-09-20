#pragma once

#include <string>
#include <tinyutils/common.hpp>

namespace tiny {
namespace utils {

/// Gets the stripped filename from an absolute full-path
///
/// @param filepath     Absolute path from which to strip the filename
/// @return String representing the filename stripped from the given fullpath
std::string GetFilename(const std::string &filepath);

/// Gets the stripped foldername from an absolute full-path
///
/// @param filepath     Absolute path from which to strip the foldername
/// @return String representing the foldername stripped from the given fullpath
std::string GetFoldername(const std::string &filepath);

/// Gets the folderpath (path with removed filename) from an absolute full-path
///
/// @param filepath     Absolute path from which to strip the folderpath
/// @return String representing the folderpath stripped from the given fullpath
std::string GetFolderpath(const std::string &filepath);

/// Gets the stripped filename from an absolute full-path, and removes the file
/// extension
///
/// @param filepath     Absolute path from which to strip the filename
/// @return String representing the filename (without extension) stripped from
/// the given fullpath
std::string GetFilenameNoExtension(const std::string &filepath);

}  // namespace utils
}  // namespace tiny
