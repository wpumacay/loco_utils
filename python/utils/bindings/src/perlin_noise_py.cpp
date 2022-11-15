#include <pybind11/pybind11.h>

#include <utils/perlin_noise.hpp>

namespace py = pybind11;

namespace utils {

// NOLINTNEXTLINE
void bindings_perlin_noise_module(py::module m) {
    {
        using Class = PerlinNoise;
        // NOLINTNEXTLINE
        py::class_<Class>(m, "PerlinNoise")
            .def_static("Init", &Class::Init)
            .def_static("Release", &Class::Release)
            .def_static("Config", &Class::Config, py::arg("num_octaves"),
                        py::arg("persistance"), py::arg("lacunarity"),
                        py::arg("noise_scale"))
            .def_static("Sample1d", &Class::Sample1d, py::arg("x"))
            .def_static("Sample2d", &Class::Sample2d, py::arg("x"),
                        py::arg("y"));
    }
}

}  // namespace utils
