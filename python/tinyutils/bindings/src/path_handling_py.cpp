
#include <pybind11/pybind11.h>

#include <tinyutils/path_handling.hpp>

namespace py = pybind11;

namespace tiny {
namespace utils {

void bindings_path_handling_module(py::module& m) {
    m.def("GetFilename", &GetFilename);
    m.def("GetFoldername", &GetFoldername);
    m.def("GetFolderpath", &GetFolderpath);
    m.def("GetFilenameNoExtension", &GetFilenameNoExtension);
}

}  // namespace utils
}  // namespace tiny
