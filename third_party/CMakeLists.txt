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

if(NOT TINYUTILS_IS_ROOT_PROJECT)
  return()
endif()

# cmake-format: off

# ------------------------------------------------------------------------------
# Spdlog is used for the logging functionality (internally uses the fmt lib)
# ------------------------------------------------------------------------------
loco_configure_git_dependency(
    TARGET spdlog
    REPO https://github.com/gabime/spdlog.git
    TAG v1.9.2
    BUILD_MODE Release
    BUILD_ARGS
      -DSPDLOG_BUILD_SHARED=OFF
      -DSPDLOG_BUILD_EXAMPLE=OFF
      -DSPDLOG_BUILD_EXAMPLE_HO=OFF
      -DSPDLOG_BUILD_TESTS=OFF
      -DSPDLOG_BUILD_TESTS_HO=OFF
      -DSPDLOG_BUILD_BENCH=OFF)

# ------------------------------------------------------------------------------
# Pybind11 is used for generating Python bindings for this project's C++ API.
# Notice that we're using a forked version in which usage of unique-ptr is
# allowed, as we use this functionality in some other parent projects
# ------------------------------------------------------------------------------
loco_configure_git_dependency(
    TARGET pybind11
    REPO https://github.com/pybind/pybind11.git
    TAG v2.9.1
    BUILD_MODE Release
    BUILD_ARGS
      -DPYBIND11_TEST=OFF
    DISCARD_UNLESS TINYUTILS_BUILD_PYTHON_BINDINGS)

# cmake-format: on
