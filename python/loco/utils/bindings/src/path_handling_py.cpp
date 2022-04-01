
#include <pybind11/pybind11.h>

#include <loco/utils/path_handling.hpp>

namespace py = pybind11;

namespace loco {
namespace utils {

// NOLINTNEXTLINE(runtime/references)
void bindings_path_handling_module(py::module& m) {
    m.def("GetFilename", &GetFilename);
    m.def("GetFoldername", &GetFoldername);
    m.def("GetFolderpath", &GetFolderpath);
    m.def("GetFilenameNoExtension", &GetFilenameNoExtension);
}

}  // namespace utils
}  // namespace loco
