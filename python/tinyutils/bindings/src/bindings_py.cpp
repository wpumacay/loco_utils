#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace tiny {
namespace utils {

extern void bindings_logging_module(py::module& m);
extern void bindings_path_handling_module(py::module& m);
extern void bindings_timing_module(py::module& m);
extern void bindings_perlin_noise_module(py::module& m);
extern void bindings_profiling_module(py::module& m);

}  // namespace utils
}  // namespace tiny

PYBIND11_MODULE(tinyutils, m) {
    tiny::utils::bindings_logging_module(m);
    tiny::utils::bindings_path_handling_module(m);
    tiny::utils::bindings_timing_module(m);
    tiny::utils::bindings_perlin_noise_module(m);
    tiny::utils::bindings_profiling_module(m);
}
