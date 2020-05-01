#pragma once

#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace tinyutils
{
    class Logger
    {
    public :

        enum class eType : uint8_t
        {
            CONSOLE_LOGGER,
            FILE_LOGGER
        };

        /// Initialized logging module to a specific mode
        static void Init( const eType& logger_type = eType::CONSOLE_LOGGER );

        /// Releases|frees all resources used by the logging module
        static void Release();

        /// Returns whether or not the logger has been properly initialized
        static bool IsActive() { return Logger::s_IsActive; }

        /// Returns the type of logger being used, either Console-logger or File-logger
        static eType GetType() { return Logger::s_Type; }

    public :

        // Core logging functionality (exposed to core-devs for internals and other checks)

        template<typename... Args>
        static void CoreTrace( fmt::basic_string_view<char> fmt, const Args&... args )
        {
            if ( s_CoreLogger )
                s_CoreLogger->trace( fmt, args... );
        }

        template<typename... Args>
        static void CoreInfo( fmt::basic_string_view<char> fmt, const Args&... args )
        {
            if ( s_CoreLogger )
                s_CoreLogger->info( fmt, args... );
        }

        template<typename... Args>
        static void CoreWarn( fmt::basic_string_view<char> fmt, const Args&... args )
        {
            if ( s_CoreLogger )
                s_CoreLogger->warn( fmt, args... );
        }

        template<typename... Args>
        static void CoreError( fmt::basic_string_view<char> fmt, const Args&... args )
        {
            if ( s_CoreLogger )
                s_CoreLogger->error( fmt, args... );
        }

        template<typename... Args>
        static void CoreCritical( fmt::basic_string_view<char> fmt, const Args&... args )
        {
            if ( s_CoreLogger )
                s_CoreLogger->critical( fmt, args... );
            exit( EXIT_FAILURE );
        }

        template<typename... Args>
        static void CoreAssert( bool is_ok, fmt::basic_string_view<char> fmt, const Args&... args )
        {
            if ( is_ok )
                return;

            if ( s_CoreLogger )
                s_CoreLogger->critical( fmt, args... );
            exit( EXIT_FAILURE );
        }

        // Client logging functionality (exposed to users for their examples and own applications)

        template<typename... Args>
        static void ClientTrace( fmt::basic_string_view<char> fmt, const Args&... args )
        {
            if ( s_ClientLogger )
                s_ClientLogger->trace( fmt, args... );
        }

        template<typename... Args>
        static void ClientInfo( fmt::basic_string_view<char> fmt, const Args&... args )
        {
            if ( s_ClientLogger )
                s_ClientLogger->info( fmt, args... );
        }

        template<typename... Args>
        static void ClientWarn( fmt::basic_string_view<char> fmt, const Args&... args )
        {
            if ( s_ClientLogger )
                s_ClientLogger->warn( fmt, args... );
        }

        template<typename... Args>
        static void ClientError( fmt::basic_string_view<char> fmt, const Args&... args )
        {
            if ( s_ClientLogger )
                s_ClientLogger->error( fmt, args... );
        }

        template<typename... Args>
        static void ClientCritical( fmt::basic_string_view<char> fmt, const Args&... args )
        {
            if ( s_ClientLogger )
                s_ClientLogger->critical( fmt, args... );
            exit( EXIT_FAILURE );
        }

        template<typename... Args>
        static void ClientAssert( bool is_ok, fmt::basic_string_view<char> fmt, const Args&... args )
        {
            if ( is_ok )
                return;

            if ( s_ClientLogger )
                s_ClientLogger->critical( fmt, args... );
            exit( EXIT_FAILURE );
        }

    private :
        /// spdlog Core logger (used for devs-related logging calls)
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        /// spdlog Client logger (used for devs-related logging calls)
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
        /// Flags used to indicate that the logging module has been properly initialized
        static bool s_IsActive;
        /// Type of logger being used by the logging module, either Console-logger or File-logger
        static eType s_Type;
    };
}

#if defined(TINYUTILS_USE_LOGS)
    #define LOG_CORE_TRACE(...)     ::tinyutils::Logger::CoreTrace(__VA_ARGS__)
    #define LOG_CORE_INFO(...)      ::tinyutils::Logger::CoreInfo(__VA_ARGS__)
    #define LOG_CORE_WARN(...)      ::tinyutils::Logger::CoreWarn(__VA_ARGS__)
    #define LOG_CORE_ERROR(...)     ::tinyutils::Logger::CoreError(__VA_ARGS__)
    #define LOG_CORE_CRITICAL(...)  ::tinyutils::Logger::CoreCritical(__VA_ARGS__)
    #define LOG_CORE_ASSERT(x,...)  ::tinyutils::Logger::CoreAssert((x), __VA_ARGS__)

    #define LOG_TRACE(...)     ::tinyutils::Logger::ClientTrace(__VA_ARGS__)
    #define LOG_INFO(...)      ::tinyutils::Logger::ClientInfo(__VA_ARGS__)
    #define LOG_WARN(...)      ::tinyutils::Logger::ClientWarn(__VA_ARGS__)
    #define LOG_ERROR(...)     ::tinyutils::Logger::ClientError(__VA_ARGS__)
    #define LOG_CRITICAL(...)  ::tinyutils::Logger::ClientCritical(__VA_ARGS__)
    #define LOG_ASSERT(x,...)  ::tinyutils::Logger::ClientAssert((x), __VA_ARGS__)

#else
    #define LOG_CORE_TRACE(...)     ((void)0)
    #define LOG_CORE_INFO(...)      ((void)0)
    #define LOG_CORE_WARN(...)      ((void)0)
    #define LOG_CORE_ERROR(...)     ((void)0)
    #define LOG_CORE_CRITICAL(...)  \
        {                           \
            exit( EXIT_FAILURE );   \
        }
    #define LOG_CORE_ASSERT(x,...)  \
        {                           \
            if ( !(x) )             \
            {                       \
                exit( EXIT_FAILURE );\
            }                       \
        }

    #define LOG_TRACE(...)     ((void)0)
    #define LOG_INFO(...)      ((void)0)
    #define LOG_WARN(...)      ((void)0)
    #define LOG_ERROR(...)     ((void)0)
    #define LOG_CRITICAL(...)       \
        {                           \
            exit( EXIT_FAILURE );   \
        }
    #define LOG_ASSERT(x,...)       \
        {                           \
            if ( !(x) )             \
            {                       \
                exit( EXIT_FAILURE );\
            }                       \
        }
#endif