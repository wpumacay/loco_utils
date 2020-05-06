
#include <pybind11/pybind11.h>
#include <tinyutils/profiling.hpp>

namespace py = pybind11;

namespace tinyutils
{
    void bindings_profiling_module( py::module& m )
    {
        py::class_<ProfilerTimer>( m, "ProfilerTimer" )
            .def( py::init( []( const std::string& timer_name, const std::string& session_name, const bool& verbose )
                {
                    return std::make_unique<ProfilerTimer>( timer_name.c_str(), session_name.c_str(), verbose );
                } ) );

        py::enum_<IProfilerSession::eType>( m, "SessionType", py::arithmetic() )
            .value( "INTERNAL", IProfilerSession::eType::INTERNAL )
            .value( "EXTERNAL_CHROME", IProfilerSession::eType::EXTERNAL_CHROME );

        py::class_<Profiler>( m, "Profiler" )
            .def_static( "Init", &Profiler::Init, py::arg( "type" ) = IProfilerSession::eType::EXTERNAL_CHROME )
            .def_static( "Release", &Profiler::Release )
            .def_static( "BeginSession", &Profiler::BeginSession )
            .def_static( "EndSession", &Profiler::EndSession )
            .def_static( "CreateTimer", []( const std::string& timer_name, const std::string& session_name )
                {
                    return std::make_unique<ProfilerTimer>( timer_name.c_str(), session_name.c_str() );
                } );
    }
}