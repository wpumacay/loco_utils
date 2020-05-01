
#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace tinyutils
{
    extern void bindings_logging_module( py::module& m );
}

PYBIND11_MODULE( tinyutils_py, m )
{
    tinyutils::bindings_logging_module( m );
}