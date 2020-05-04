
#include <pybind11/pybind11.h>
#include <tinyutils/path_handling.hpp>

namespace py = pybind11;

namespace tinyutils
{
    void bindings_path_handling_module( py::module& m )
    {
        m.def( "GetFilename", &tinyutils::GetFilename );
        m.def( "GetFoldername", &tinyutils::GetFoldername );
        m.def( "GetFolderpath", &tinyutils::GetFolderpath );
        m.def( "GetFilenameNoExtension", &tinyutils::GetFilenameNoExtension );
    }
}