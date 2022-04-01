#include <pybind11/pybind11.h>

#include <loco/utils/perlin_noise.hpp>

namespace py = pybind11;

namespace loco {
namespace utils {

// NOLINTNEXTLINE(runtime/references)
void bindings_perlin_noise_module(py::module& m) {
    py::class_<PerlinNoise>(m, "PerlinNoise")
        .def_static("Init", &PerlinNoise::Init)
        .def_static("Release", &PerlinNoise::Release)
        .def_static("Config", &PerlinNoise::Config, py::arg("num_octaves"),
                    py::arg("persistance"), py::arg("lacunarity"),
                    py::arg("noise_scale"))
        .def_static("Sample1d", &PerlinNoise::Sample1d, py::arg("x"))
        .def_static("Sample2d", &PerlinNoise::Sample2d, py::arg("x"),
                    py::arg("y"));
}

}  // namespace utils
}  // namespace loco
