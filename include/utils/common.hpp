#pragma once

// -----------------------------------------------------------------------------
// Language detection adapted from https://github.com/g-truc/glm

// clang-format off

#if defined(UTILS_FORCE_INLINE)
    #if defined(UTILS_COMPILER_CLANG) || defined(UTILS_COMPILER_GCC)
        #define UTILS_INLINE inline __attribute__((__always_inline__))
    #elif defined(UTILS_COMPILER_MSVC)
        #define UTILS_INLINE __forceinline
    #else
        #define UTILS_INLINE inline
    #endif
#else
    #define UTILS_INLINE
#endif

#if defined(UTILS_COMPILER_CLANG) || defined(UTILS_COMPILER_GCC)
    #define UTILS_NEVER_INLINE __attribute__((noinline))
#elif defined(UTILS_COMPILER_MSVC)
    #define UTILS_NEVER_INLINE __declspec(noinline)
#else
    #define UTILS_NEVER_INLINE
#endif

#define UTILS_LANG_CXX98_FLAG (1 << 1)
#define UTILS_LANG_CXX03_FLAG (1 << 2)
#define UTILS_LANG_CXX0X_FLAG (1 << 3)
#define UTILS_LANG_CXX11_FLAG (1 << 4)
#define UTILS_LANG_CXX14_FLAG (1 << 5)
#define UTILS_LANG_CXX17_FLAG (1 << 6)
#define UTILS_LANG_CXX20_FLAG (1 << 7)

#define UTILS_LANG_CXX98 UTILS_LANG_CXX98_FLAG
#define UTILS_LANG_CXX03 (UTILS_LANG_CXX98 | UTILS_LANG_CXX03_FLAG)
#define UTILS_LANG_CXX0X (UTILS_LANG_CXX03 | UTILS_LANG_CXX0X_FLAG)
#define UTILS_LANG_CXX11 (UTILS_LANG_CXX0X | UTILS_LANG_CXX11_FLAG)
#define UTILS_LANG_CXX14 (UTILS_LANG_CXX11 | UTILS_LANG_CXX14_FLAG)
#define UTILS_LANG_CXX17 (UTILS_LANG_CXX14 | UTILS_LANG_CXX17_FLAG)
#define UTILS_LANG_CXX20 (UTILS_LANG_CXX17 | UTILS_LANG_CXX20_FLAG)

#if defined(UTILS_FORCE_CXX20)
    #define UTILS_LANG UTILS_LANG_CXX20
#elif defined(UTILS_FORCE_CXX17)
    #define UTILS_LANG UTILS_LANG_CXX17
#elif defined(UTILS_FORCE_CXX14)
    #define UTILS_LANG UTILS_LANG_CXX14
#elif defined(UTILS_FORCE_CXX11)
    #define UTILS_LANG UTILS_LANG_CXX11
#else
    #if __cplusplus > 201703L
        #define UTILS_LANG UTILS_LANG_CXX20
    #elif __cplusplus == 201703L
        #define UTILS_LANG UTILS_LANG_CXX17
    #elif __cplusplus == 201402L
        #define UTILS_LANG UTILS_LANG_CXX14
    #elif __cplusplus == 201103L
        #define UTILS_LANG UTILS_LANG_CXX11
    #else
        #error "C++ standard must be one of 11, 14, 17, and 20"
    #endif
#endif

// [[nodiscard]]
#if UTILS_LANG & UTILS_LANG_CXX17_FLAG
    #define UTILS_NODISCARD [[nodiscard]]
#else
    #define UTILS_NODISCARD
#endif

#if defined _WIN32 || defined __CYGWIN__
    #define UTILS_DLL_EXPORT __declspec(dllexport)
    #define UTILS_DLL_IMPORT __declspec(dllimport)
    #define UTILS_DLL_LOCAL
#else
    #if __GNUC__ >= 4
        #define UTILS_DLL_EXPORT __attribute__ ((visibility ("default")))
        #define UTILS_DLL_IMPORT __attribute__ ((visibility ("default")))
        #define UTILS_DLL_LOCAL __attribute__ ((visibility ("hidden")))
    #else
        #define UTILS_DLL_EXPORT
        #define UTILS_DLL_IMPORT
        #define UTILS_DLL_LOCAL
    #endif
#endif

#define UTILS_DECL UTILS_NODISCARD

#ifdef UTILS_STATIC
    #define UTILS_API
    #define UTILS_LOCAL
#else
    #ifdef UTILS_DLL_EXPORTS
        #define UTILS_API UTILS_DLL_EXPORT
    #else
        #define UTILS_API UTILS_DLL_IMPORT
    #endif
    #define UTILS_LOCAL UTILS_DLL_LOCAL
#endif

// clang-format on

// -----------------------------------------------------------------------------

#include <cstdint>
#include <cstdlib>
#include <memory>
#include <string>
#include <vector>

namespace utils {

/// Divides a given string into substrings using the given character separator
///
/// \param txt          String to be splitted into substrings
/// \param separator    Character separator used for the splitting process
/// \return Vector of strings that form the initial string, after splitting
/// using the separator
UTILS_API auto Split(const std::string &txt, char separator = '/')
    -> std::vector<std::string>;

/// Gets the string representation of the hex-address of a given pointer
///
/// \param ptr  Pointer from whom to get the hex-address representation
/// \return String representation of the hex-address of the pointer
UTILS_API auto PointerToHexAddress(const void *ptr) -> std::string;

/// Returns a string with the contents of a given file
UTILS_API auto GetFileContents(const char *filepath) -> std::string;

}  // namespace utils

//----------------------------------------------------------------------------//
//       Use make_unique extension if the compiler is c++11 or older          //
//----------------------------------------------------------------------------//
#if __cplusplus <= 201103L

// make_unique implementation from Stephan T. Lavavej
// post:
// https://stackoverflow.com/questions/7038357/make-unique-and-perfect-forwarding
// url: https://isocpp.org/files/papers/N3656.txt
#include <cstddef>
#include <type_traits>
#include <utility>

namespace std {
template <class T>
struct _Unique_if {                        // NOLINT
    typedef unique_ptr<T> _Single_object;  // NOLINT
};

template <class T>
struct _Unique_if<T[]> {                     // NOLINT
    typedef unique_ptr<T[]> _Unknown_bound;  // NOLINT
};

template <class T, size_t N>
struct _Unique_if<T[N]> {       // NOLINT
    typedef void _Known_bound;  // NOLINT
};

template <class T, class... Args>  // NOLINTNEXTLINE
typename _Unique_if<T>::_Single_object make_unique(Args &&...args) {
    return unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template <class T>
typename _Unique_if<T>::_Unknown_bound make_unique(size_t n) {  // NOLINT
    typedef typename remove_extent<T>::type U;                  // NOLINT
    return unique_ptr<T>(new U[n]());
}

template <class T, class... Args>  // NOLINTNEXTLINE
typename _Unique_if<T>::_Known_bound make_unique(Args &&...) = delete;
}  // namespace std

#endif /* checks that we're using c++11 or older */

//----------------------------------------------------------------------------//
//  Function-name helper macro, directive depends on the compiler being used  //
//----------------------------------------------------------------------------//
#if defined(__GNUC__)
// NOLINTNEXTLINE @todo(wilbert): might be possible to change to constexpr?
#define __FUNCTION_NAME__ __PRETTY_FUNCTION__
#elif defined(__clang__)
#define __FUNCTION_NAME__ __FUNCTION__
#elif defined(_MSC_VER)
#define __FUNCTION_NAME__ __FUNCSIG__
#endif

//----------------------------------------------------------------------------//
//              Smart pointer helper macros for class-declarations            //
//----------------------------------------------------------------------------//
// Macros based on helper-macros from habitat-sim repo
// repo: https://github.com/facebookresearch/habitat-sim
// snippet-url:
// https://github.com/facebookresearch/habitat-sim/blob/3d810b9c006976be8bd15b4ca8cf5170c5ad398d/src/esp/core/esp.h#L108
// NOLINTNEXTLINE @todo(wilbert) might be possible to change to contexpr?
#define DEFINE_SMART_POINTERS(Classname)                                  \
 public:                                                                  \
    using ptr = std::shared_ptr<Classname>;                               \
    using cptr = std::shared_ptr<const Classname>;                        \
    using uptr = std::unique_ptr<Classname>;                              \
    using cuptr = std::unique_ptr<const Classname>;                       \
    template <typename... Targs>                                          \
    static inline auto Create(Targs &&...args)->ptr {                     \
        return std::make_shared<Classname>(std::forward<Targs>(args)...); \
    }                                                                     \
    template <typename... Targs>                                          \
    static inline auto CreateUnique(Targs &&...args)->uptr {              \
        return std::make_unique<Classname>(std::forward<Targs>(args)...); \
    }

//----------------------------------------------------------------------------//
//                    RAII Rule of five helper-macro                          //
//----------------------------------------------------------------------------//
// Macro based on helper macro from drake repo:
// https://github.com/RobotLocomotion/drake/blob/master/common/drake_copyable.h
// NOLINTNEXTLINE
#define NO_COPY_NO_MOVE_NO_ASSIGN(Classname)                 \
 public:                                                     \
    Classname(const Classname &) = delete;                   \
    auto operator=(const Classname &)->Classname & = delete; \
    Classname(Classname &&) = delete;                        \
    auto operator=(Classname &&)->Classname & = delete;

// NOLINTNEXTLINE
#define DEFAULT_COPY_AND_MOVE_AND_ASSIGN(Classname)           \
    Classname(const Classname &) = default;                   \
    auto operator=(const Classname &)->Classname & = default; \
    Classname(Classname &&) = default;                        \
    auto operator=(Classname &&)->Classname & = default;
