#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace loco {
namespace utils {

// @todo(wilbert): check if should actually pass by reference, or should pass
// by const-reference instead

// Disable linting for the following extern declarations, as we're using the
// pattern of pass by reference used by some pybind11 examples

// NOLINTNEXTLINE(runtime/references)
extern void bindings_logging_module(py::module& m);
// NOLINTNEXTLINE(runtime/references)
extern void bindings_path_handling_module(py::module& m);
// NOLINTNEXTLINE(runtime/references)
extern void bindings_timing_module(py::module& m);
// NOLINTNEXTLINE(runtime/references)
extern void bindings_perlin_noise_module(py::module& m);
// NOLINTNEXTLINE(runtime/references)
extern void bindings_profiling_module(py::module& m);

}  // namespace utils
}  // namespace loco

// NOLINTNEXTLINE @todo(wilbert): check how other projects handle this part
PYBIND11_MODULE(utils, m) {
    loco::utils::bindings_logging_module(m);
    loco::utils::bindings_path_handling_module(m);
    loco::utils::bindings_timing_module(m);
    loco::utils::bindings_perlin_noise_module(m);
    loco::utils::bindings_profiling_module(m);
}
