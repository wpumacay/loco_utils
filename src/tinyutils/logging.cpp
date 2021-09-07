#include <cstdio>
#include <tinyutils/logging.hpp>

namespace tiny {
namespace utils {

std::shared_ptr<spdlog::logger> Logger::s_CoreLogger = nullptr;

std::shared_ptr<spdlog::logger> Logger::s_ClientLogger = nullptr;

bool Logger::s_IsActive = false;

Logger::eType Logger::s_Type = Logger::eType::CONSOLE_LOGGER;

void Logger::Init(const Logger::eType &logger_type) {
    if (s_IsActive) return;

    s_IsActive = true;
    s_Type = logger_type;

    spdlog::set_pattern("%^[%T] %n: %v%$");
    if (s_Type == Logger::eType::CONSOLE_LOGGER) {
        s_CoreLogger = spdlog::stdout_color_mt("CORE");
        s_CoreLogger->set_level(spdlog::level::trace);
        s_ClientLogger = spdlog::stdout_color_mt("USER");
        s_ClientLogger->set_level(spdlog::level::trace);
    } else {
        try {
            s_CoreLogger = spdlog::basic_logger_mt("CORE", "./core_logs.txt");
            s_CoreLogger->set_level(spdlog::level::trace);
            s_ClientLogger = spdlog::basic_logger_mt("USER", "./user_logs.txt");
            s_ClientLogger->set_level(spdlog::level::trace);
        } catch (const spdlog::spdlog_ex &ex) {
            printf("Logger initialization FAILED: %s\n", ex.what());
        }
    }
}

void Logger::Release() {
    // Clean referencess. If using only macros for logging then all other
    // references should have been deleted once the macro finish execution (so,
    // only these two refs. are active).
    s_CoreLogger = nullptr;
    s_ClientLogger = nullptr;
    s_IsActive = false;
    spdlog::drop_all();
}

}  // namespace utils
}  // namespace tiny
