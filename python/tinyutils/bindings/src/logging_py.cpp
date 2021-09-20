#include <pybind11/pybind11.h>

#include <tinyutils/logging.hpp>

namespace py = pybind11;

namespace tiny {
namespace utils {

// NOLINTNEXTLINE(runtime/references)
void bindings_logging_module(py::module& m) {
    py::enum_<Logger::eType>(m, "LoggerType", py::arithmetic())
        .value("CONSOLE_LOGGER", Logger::eType::CONSOLE_LOGGER)
        .value("FILE_LOGGER", Logger::eType::FILE_LOGGER);

    py::class_<Logger>(m, "Logger")
        .def_static("Init", &Logger::Init,
                    py::arg("type") = Logger::eType::CONSOLE_LOGGER)
        .def_static("Release", &Logger::Release)
        .def_static("IsActive", &Logger::IsActive)
        .def_static("GetType", &Logger::GetType)
        .def_static("CoreTrace",
                    [](const std::string& str_msg) { LOG_CORE_TRACE(str_msg); })
        .def_static("CoreInfo",
                    [](const std::string& str_msg) { LOG_CORE_INFO(str_msg); })
        .def_static("CoreWarn",
                    [](const std::string& str_msg) { LOG_CORE_WARN(str_msg); })
        .def_static("CoreError",
                    [](const std::string& str_msg) { LOG_CORE_ERROR(str_msg); })
        .def_static(
            "CoreCritical",
            [](const std::string& str_msg) { LOG_CORE_CRITICAL(str_msg); })
        .def_static("CoreAssert",
                    [](const bool& is_ok, const std::string& str_msg) {
                        LOG_CORE_ASSERT(is_ok, str_msg);
                    })
        .def_static("Trace",
                    [](const std::string& str_msg) { LOG_TRACE(str_msg); })
        .def_static("Info",
                    [](const std::string& str_msg) { LOG_INFO(str_msg); })
        .def_static("Warn",
                    [](const std::string& str_msg) { LOG_WARN(str_msg); })
        .def_static("Error",
                    [](const std::string& str_msg) { LOG_ERROR(str_msg); })
        .def_static("Critical",
                    [](const std::string& str_msg) { LOG_CRITICAL(str_msg); })
        .def_static("Assert",
                    [](const bool& is_ok, const std::string& str_msg) {
                        LOG_ASSERT(is_ok, str_msg);
                    });
}

}  // namespace utils
}  // namespace tiny
