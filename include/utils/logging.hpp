#pragma once

#include <utils/common.hpp>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace utils {

class Logger {
 public:
    enum class eType : uint8_t { CONSOLE_LOGGER, FILE_LOGGER };

    /// Initialized logging module to a specific mode
    static void Init(const eType& logger_type = eType::CONSOLE_LOGGER);

    /// Releases|frees all resources used by the logging module
    static void Release();

    /// Returns whether or not the logger has been properly initialized
    static auto IsActive() -> bool { return Logger::s_IsActive; }

    /// Returns the type of logger being used, either Console-logger or
    /// File-logger
    static auto GetType() -> eType { return Logger::s_Type; }

 public:
    // Core logging fcn calls (exposed to devs for internals and other checks)

    template <typename... Args>
    static void CoreTrace(fmt::basic_string_view<char> fmt,
                          const Args&... args) {
        if (!Logger::s_IsActive) {
            Logger::Init();
        }
        if (s_CoreLogger) {
            s_CoreLogger->trace(fmt, args...);
        }
    }

    template <typename... Args>
    static void CoreInfo(fmt::basic_string_view<char> fmt,
                         const Args&... args) {
        if (!Logger::s_IsActive) {
            Logger::Init();
        }
        if (s_CoreLogger) {
            s_CoreLogger->info(fmt, args...);
        }
    }

    template <typename... Args>
    static void CoreWarn(fmt::basic_string_view<char> fmt,
                         const Args&... args) {
        if (!Logger::s_IsActive) {
            Logger::Init();
        }
        if (s_CoreLogger) {
            s_CoreLogger->warn(fmt, args...);
        }
    }

    template <typename... Args>
    static void CoreError(fmt::basic_string_view<char> fmt,
                          const Args&... args) {
        if (!Logger::s_IsActive) {
            Logger::Init();
        }
        if (s_CoreLogger) {
            s_CoreLogger->error(fmt, args...);
        }
    }

    template <typename... Args>
    static void CoreCritical(fmt::basic_string_view<char> fmt,
                             const Args&... args) {
        if (!Logger::s_IsActive) {
            Logger::Init();
        }
        if (s_CoreLogger) {
            s_CoreLogger->critical(fmt, args...);
        }
        exit(EXIT_FAILURE);
    }

    template <typename... Args>
    static void CoreAssert(bool is_not_ok, fmt::basic_string_view<char> fmt,
                           const Args&... args) {
        if (!is_not_ok) {
            return;
        }

        if (!Logger::s_IsActive) {
            Logger::Init();
        }
        if (s_CoreLogger) {
            s_CoreLogger->critical(fmt, args...);
        }
        exit(EXIT_FAILURE);
    }

    // Client logging functionality (exposed to users for their examples and own
    // applications)

    template <typename... Args>
    static void ClientTrace(fmt::basic_string_view<char> fmt,
                            const Args&... args) {
        if (!Logger::s_IsActive) {
            Logger::Init();
        }
        if (s_ClientLogger) {
            s_ClientLogger->trace(fmt, args...);
        }
    }

    template <typename... Args>
    static void ClientInfo(fmt::basic_string_view<char> fmt,
                           const Args&... args) {
        if (!Logger::s_IsActive) {
            Logger::Init();
        }
        if (s_ClientLogger) {
            s_ClientLogger->info(fmt, args...);
        }
    }

    template <typename... Args>
    static void ClientWarn(fmt::basic_string_view<char> fmt,
                           const Args&... args) {
        if (!Logger::s_IsActive) {
            Logger::Init();
        }
        if (s_ClientLogger) {
            s_ClientLogger->warn(fmt, args...);
        }
    }

    template <typename... Args>
    static void ClientError(fmt::basic_string_view<char> fmt,
                            const Args&... args) {
        if (!Logger::s_IsActive) {
            Logger::Init();
        }
        if (s_ClientLogger) {
            s_ClientLogger->error(fmt, args...);
        }
    }

    template <typename... Args>
    static void ClientCritical(fmt::basic_string_view<char> fmt,
                               const Args&... args) {
        if (!Logger::s_IsActive) {
            Logger::Init();
        }
        if (s_ClientLogger) {
            s_ClientLogger->critical(fmt, args...);
        }
        exit(EXIT_FAILURE);
    }

    template <typename... Args>
    static void ClientAssert(bool is_not_ok, fmt::basic_string_view<char> fmt,
                             const Args&... args) {
        if (!is_not_ok) {
            return;
        }

        if (!Logger::s_IsActive) {
            Logger::Init();
        }
        if (s_ClientLogger) {
            s_ClientLogger->critical(fmt, args...);
        }
        exit(EXIT_FAILURE);
    }

 private:
    // @todo(wilbert): The variables below are not actually accessible, but
    // could should think about it making it const? (will disable lint for now)

    /// spdlog Core logger (used for devs-related logging calls)
    static std::shared_ptr<spdlog::logger> s_CoreLogger;  // NOLINT
    /// spdlog Client logger (used for devs-related logging calls)
    static std::shared_ptr<spdlog::logger> s_ClientLogger;  // NOLINT
    /// Flags used to indicate that the logging module has been properly
    /// initialized
    static bool s_IsActive;  // NOLINT
    /// Type of logger being used by the logging module, either Console-logger
    /// or File-logger
    static eType s_Type;  // NOLINT
};

}  // namespace utils

// @todo(wilbert): Should actually try to change these variadic macros to
// constexpr variadic template functions (for now will just disable linting)

// NOLINTNEXTLINE
#define LOG_CORE_TRACE(...) ::utils::Logger::CoreTrace(__VA_ARGS__)
// NOLINTNEXTLINE
#define LOG_CORE_INFO(...) ::utils::Logger::CoreInfo(__VA_ARGS__)
// NOLINTNEXTLINE
#define LOG_CORE_WARN(...) ::utils::Logger::CoreWarn(__VA_ARGS__)
// NOLINTNEXTLINE
#define LOG_CORE_ERROR(...) ::utils::Logger::CoreError(__VA_ARGS__)
// NOLINTNEXTLINE
#define LOG_CORE_CRITICAL(...) ::utils::Logger::CoreCritical(__VA_ARGS__)
// NOLINTNEXTLINE
#define LOG_CORE_ASSERT(x, ...) ::utils::Logger::CoreAssert(!(x), __VA_ARGS__)

// NOLINTNEXTLINE
#define LOG_TRACE(...) ::utils::Logger::ClientTrace(__VA_ARGS__)
// NOLINTNEXTLINE
#define LOG_INFO(...) ::utils::Logger::ClientInfo(__VA_ARGS__)
// NOLINTNEXTLINE
#define LOG_WARN(...) ::utils::Logger::ClientWarn(__VA_ARGS__)
// NOLINTNEXTLINE
#define LOG_ERROR(...) ::utils::Logger::ClientError(__VA_ARGS__)
// NOLINTNEXTLINE
#define LOG_CRITICAL(...) ::utils::Logger::ClientCritical(__VA_ARGS__)
// NOLINTNEXTLINE
#define LOG_ASSERT(x, ...) ::utils::Logger::ClientAssert(!(x), __VA_ARGS__)
