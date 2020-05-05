#pragma once

#include <cstdlib>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <cassert>
#include <memory>

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

// Use make_unique extension if the compiler is c++11 or older (make_unique was introduced in c++14)
#if __cplusplus <= 201103L

// make_unique implementation from Stephan T. Lavavej
// post: https://stackoverflow.com/questions/7038357/make-unique-and-perfect-forwarding
// url: https://isocpp.org/files/papers/N3656.txt
#include <cstddef>
#include <type_traits>
#include <utility>

namespace std {
    template<class T> struct _Unique_if {
        typedef unique_ptr<T> _Single_object;
    };

    template<class T> struct _Unique_if<T[]> {
        typedef unique_ptr<T[]> _Unknown_bound;
    };

    template<class T, size_t N> struct _Unique_if<T[N]> {
        typedef void _Known_bound;
    };

    template<class T, class... Args>
        typename _Unique_if<T>::_Single_object
        make_unique(Args&&... args) {
            return unique_ptr<T>(new T(std::forward<Args>(args)...));
        }

    template<class T>
        typename _Unique_if<T>::_Unknown_bound
        make_unique(size_t n) {
            typedef typename remove_extent<T>::type U;
            return unique_ptr<T>(new U[n]());
        }

    template<class T, class... Args>
        typename _Unique_if<T>::_Known_bound
        make_unique(Args&&...) = delete;
}

#endif /* checks that we're using c++11 or older */