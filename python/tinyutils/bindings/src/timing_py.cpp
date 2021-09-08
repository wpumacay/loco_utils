#include <pybind11/pybind11.h>

#include <tinyutils/timing.hpp>

namespace py = pybind11;

namespace tiny {
namespace utils {

void bindings_timing_module(py::module& m) {
    // Bindings for ClockEvent struct
    {
        py::class_<ClockEvent>(m, "ClockEvent")
            .def_readwrite("name", &ClockEvent::name)
            .def_readwrite("time_start", &ClockEvent::time_start)
            .def_readwrite("time_stop", &ClockEvent::time_stop)
            .def_readwrite("time_duration", &ClockEvent::time_duration);
    }

    // Bindings for Clock module
    {
        py::class_<Clock>(m, "Clock")
            .def_static("Init", &Clock::Init)
            .def_static("Release", &Clock::Release)
            .def_static("Tick", &Clock::Tick,
                        py::arg("event_name") = MAIN_EVENT)
            .def_static("Tock", &Clock::Tock,
                        py::arg("event_name") = MAIN_EVENT)
            .def_static("GetEvent", &Clock::GetEvent)
            .def_static("GetWallTime", &Clock::GetWallTime)
            .def_static("GetTimeStep", &Clock::GetTimeStep)
            .def_static("GetAvgTimeStep", &Clock::GetAvgTimeStep)
            .def_static("GetFps", &Clock::GetFps)
            .def_static("GetAvgFps", &Clock::GetAvgFps);
    }
}

}  // namespace utils
}  // namespace tiny
