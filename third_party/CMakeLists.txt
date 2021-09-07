# ~~~
# CMake configuration for third-party dependencies.
#
# Based on the superbuild script by jeffamstutz for ospray
# https://github.com/jeffamstutz/superbuild_ospray/blob/main/macros.cmake
# ~~~

if(NOT TINYUTILS_IS_ROOT_PROJECT)
  tuMessage("Third-party deps. should be handled by root" LOG_LEVEL TRACE)
  return()
endif()

# Grab spdlog, as we need it as logging library (uses internally the fmt lib.)
# cmake-format: off
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
# cmake-format: on

# Grab Pybind11 to create Pythin bindings for this project :D
if(TINYUTILS_BUILD_PYTHON_BINDINGS)
  # cmake-format: off
  tuConfigureGitDependency(
      TARGET pybind11
      REPO https://github.com/pybind/pybind11.git
      TAG v2.7.1
      BUILD_MODE Release
      BUILD_ARGS -DPYBIND11_TEST=OFF)
  # cmake-format: on

  # Notify the user in casethis dependency couldn't be found
  if(NOT pybind11_FOUND)
    tuMessage("Requested Python bindings, but couldn't find Pybind11 :("
              LOG_LEVEL WARNING)
    set(TINYUTILS_BUILD_PYTHON_BINDINGS
        OFF
        PARENT_SCOPE)
  endif()
endif()
