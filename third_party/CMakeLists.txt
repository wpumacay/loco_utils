# ~~~
# CMake configuration for third-party dependencies.
#
# Dependencies:
# * spdlog
# * pybind11
#
# Based on the superbuild script by jeffamstutz for ospray
# https://github.com/jeffamstutz/superbuild_ospray/blob/main/macros.cmake
# ~~~

# cmake-format: off

# ------------------------------------------------------------------------------
# Spdlog is used for the logging functionality (internally uses the fmt lib)
# ------------------------------------------------------------------------------
tuConfigureGitDependency(
    TARGET spdlog
    REPO https://github.com/gabime/spdlog.git
    TAG v1.9.2
    BUILD_MODE Release
    BUILD_ARGS
      -DSPDLOG_BUILD_SHARED OFF CACHE BOOL "Build as a static library"
      -DSPDLOG_BUILD_EXAMPLE OFF CACHE BOOL "Don't build spdlog examples"
      -DSPDLOG_BUILD_EXAMPLE_HO OFF CACHE BOOL "Don't build (ho-flag) examples"
      -DSPDLOG_BUILD_TESTS OFF CACHE BOOL "Don't build spdlog unittests"
      -DSPDLOG_BUILD_TESTS_HO OFF CACHE BOOL "Don't build (ho-flag) unittests"
      -DSPDLOG_BUILD_BENCH OFF CACHE BOOL "Don't build spdlog benchmarks")

# ------------------------------------------------------------------------------
# Pybind11 is used for generating Python bindings for this project's C++ API.
# Notice that we're using a forked version in which usage of unique-ptr is
# allowed, as we use this functionality in some other parent projects
# ------------------------------------------------------------------------------
tuConfigureGitDependency(
    TARGET pybind11
    REPO https://github.com/pybind/pybind11.git
    TAG v2.7.1
    BUILD_MODE Release
    BUILD_ARGS
      -DPYBIND11_TEST=OFF
    DISCARD_UNLESS TINYUTILS_BUILD_PYTHON_BINDINGS)

# cmake-format: on
