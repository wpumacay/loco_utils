#pragma once

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <cassert>

namespace tinyutils
{
    /// Divides a given string into substrings using the given character separator
    ///
    /// @param txt          String to be splitted into substrings
    /// @param separator    Character separator used for the splitting process
    /// @return Vector of strings that form the initial string, after splitting using the separator
    std::vector<std::string> Split( const std::string& txt, char separator = '/' );

    /// Gets the string representation of the hex-address of a given pointer
    ///
    /// @param ptr  Pointer from whom to get the hex-address representation
    /// @return String representation of the hex-address of the pointer
    std::string PointerToHexAddress( const void* ptr );
}