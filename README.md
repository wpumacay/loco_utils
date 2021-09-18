# Tiny-Utils

A small set of C/C++ helper functionality that is used alongside various of my projects

## Build Status

| Build   | Status                                                                                                                                                                      |
| ------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Ubuntu  | [![ci-linux](https://github.com/wpumacay/tiny_utils/actions/workflows/ci-linux.yml/badge.svg)](https://github.com/wpumacay/tiny_utils/actions/workflows/ci-linux.yml)       |
| Windows | [![ci-windows](https://github.com/wpumacay/tiny_utils/actions/workflows/ci-windows.yml/badge.svg)](https://github.com/wpumacay/tiny_utils/actions/workflows/ci-windows.yml) |
| MacOS   | [![ci-macos](https://github.com/wpumacay/tiny_utils/actions/workflows/ci-macos.yml/badge.svg)](https://github.com/wpumacay/tiny_utils/actions/workflows/ci-macos.yml)       |

## How to Use

This is a `CMake` based project, and all dependencies are managed by this tool
itself (i.e., we make use of `FetchContent`), so no additional dependencies have
to be installed. So, there are two ways you can use this project alongside yours:

* Include via `add_subdirectory`: This is the quickest way to integrate the
  project alongside yours. You'll have to download the source code to some folder
  either by cloning the repo, or by adding it as a `git-submodule`. Either way,
  let's say you placed the source into `SOME_FOLDER/tiny_utils`. Then you'll just
  have to do the following in some part of your CMake setup:

  ```cmake
    # On some part of your CMake configuration rules
    add_subdirectory(SOME_FOLDER/tiny_utils)
  ```

* Get it via `FetchContent`: This is an option that doesn't require for you to
  directly clone or link the repo. You can use `FetchContent` and `FetchContent_MakeAvailable`
  to add this project directly into your CMake build workflow, as shown below:

  ```cmake
    # Set where to place source-code and where to place binaries/logs
    set(SOME_SRC_PATH "Some-source-path-here")
    set(SOME_BUILD_PATH "Some-build-path-here")
    # Define some extra cmake-args to pass to this project (tiny_utils)
    set(SOME_CMAKE_ARGS "-DTINYUTILS_BUILD_EXAMPLES=OFF -DTINY_UTILS_BUILD_DOCS=OFF")
    # On some part where you setup third-party dependencies
    FetchContent(tiny_utils
        GIT_REPOSITORY "https://github.com/wpumacay/tiny_utils.git"
        GIT_PROGRESS TRUE
        USES_TERMINAL_DOWNLOAD TRUE
        PREFIX "${SOME_SRC_PATH}/tiny_utils"
        DOWNLOAD_DIR "${SOME_SRC_PATH}/tiny_utils"
        SOURCE_DIR "${SOME_SRC_PATH}/tiny_utils/source"
        BINARY_DIR "${SOME_BUILD_PATH}/tiny_utils/build"
        STAMP_DIR "${SOME_BUILD_PATH}/tiny_utils/stamp"
        TMP_DIR "${SOME_BUILD_PATH}/tiny_utils/tmp"
        CMAKE_ARGS -DCMAKE_BUILD_TYPE=Release
                   -DCMAKE_GENERATOR=${CMAKE_GENERATOR}
                   -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
                   -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
                   -DCMAKE_INSTALL_INCLUDEDIR=${CMAKE_INSTALL_INCLUDEDIR}
                   -DCMAKE_INSTALL_LIBDIR=${CMAKE_INSTALL_LIBDIR}
                   -DCMAKE_INSTALL_DOCDIR=${CMAKE_INSTALL_DOCDIR}
                   -DCMAKE_INSTALL_BINDIR=${CMAKE_INSTALL_BINDIR}
                   -DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}
                   ${SOME_CMAKE_ARGS}
        BUILD_ALWAYS OFF)
  ```

* Install and use `find_package`: This last option is not available yet, but once
  we add the install rules for the CMake project, you'll be able to just **install**
  the library into your system, import it using **find_package**, and **linking**
  using the exposed `TinyUtilsCpp` target, as shown below:

  * *Installing the project*

  ```shell
    mkdir build
    cmake -S . -B build
    cmake --build build
    cmake --install build
  ```

  * *Including using `find_package`*

  ```cmake
    # Locate the installed project using find_package()
    find_package(TinyUtils REQUIRED)
  ```

## Install the Python bindings

There are some Python bindings (generated using Pybind11), which you can install
using the provided `setup.py` script. Just run either `python setup.py install`,
or `pip install --verbose .` (add verbose just to check if everything went well)

```shell
  # Install python bindings
  # Option-1
  python setup.py install
  # Option-2
  pip install --verbose .
```
