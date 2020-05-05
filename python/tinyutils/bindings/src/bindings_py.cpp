
#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace tinyutils
{
    extern void bindings_logging_module( py::module& m );
    extern void bindings_path_handling_module( py::module& m );
    extern void bindings_timing_module( py::module& m );
    extern void bindings_perlin_noise_module( py::module& m );
    //// extern void bindings_profiling_module( py::module& m );
}

PYBIND11_MODULE( tinyutils_py, m )
{
    tinyutils::bindings_logging_module( m );
    tinyutils::bindings_path_handling_module( m );
    tinyutils::bindings_timing_module( m );
    tinyutils::bindings_perlin_noise_module( m );
}