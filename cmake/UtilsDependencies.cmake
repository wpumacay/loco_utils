# ~~~
# CMake configuration for third-party dependencies.
#
# Dependencies:
# * spdlog
# * pybind11
# * catch2
#
# - Based on the superbuild script by jeffamstutz for ospray
#   https://github.com/jeffamstutz/superbuild_ospray/blob/main/macros.cmake
# - Based on MuJoCo's helper find_or_fetch content macro
#   https://github.com/deepmind/mujoco/blob/main/cmake/FindOrFetch.cmake
# ~~~

set(UTILS_DEP_VERSION_spdlog
    ad0e89cbfb4d0c1ce4d097e134eb7be67baebb36 # Release 1.10.0
    CACHE STRING
          "Version of spdlog to be fetched (dependency of loco-utils library)")

set(UTILS_DEP_VERSION_catch2
    182c910b4b63ff587a3440e08f84f70497e49a81 # Release 2.13.10
    CACHE STRING "Version of Catch2 to be fetched (used for unittests)")

set(UTILS_DEP_VERSION_pybind11
    8a099e44b3d5f85b20f05828d919d2332a8de841 # Release v2.11.1
    CACHE STRING "Version of PyBind11 to be fetched (used for python bindings)")

set(UTILS_DEP_VERSION_tinyobjloader
    1421a10d6ed9742f5b2c1766d22faa6cfbc56248
    CACHE STRING "Version of 'tinyobjloader' to be fetched")

set(UTILS_DEP_VERSION_math
    465647cac069bb1268bccc8adc33157e982f473b # Version v0.6.10
    CACHE STRING "Version of internal math repo to be fetched")

mark_as_advanced(UTILS_DEP_VERSION_spdlog)
mark_as_advanced(UTILS_DEP_VERSION_catch2)
mark_as_advanced(UTILS_DEP_VERSION_pybind11)
mark_as_advanced(UTILS_DEP_VERSION_tinyobjloader)
mark_As_advanced(UTILS_DEP_VERSION_math)

# cmake-format: off
# ------------------------------------------------------------------------------
# Catch2 is used for generating unittests for our C++ codebase
# ------------------------------------------------------------------------------

set(CATCH_INSTALL_DOCS OFF CACHE BOOL "" FORCE)
set(CATCH_INSTALL_EXTRAS OFF CACHE BOOL "" FORCE)
set(CATCH_DEVELOPMENT_BUILD OFF CACHE BOOL "" FORCE)

loco_find_or_fetch_dependency(
  USE_SYSTEM_PACKAGE FALSE
  PACKAGE_NAME Catch2
  LIBRARY_NAME catch2
  GIT_REPO https://github.com/catchorg/Catch2.git
  GIT_TAG ${UTILS_DEP_VERSION_catch2}
  GIT_PROGRESS FALSE
  GIT_SHALLOW TRUE
  TARGETS Catch2::Catch2
  EXCLUDE_FROM_ALL)

# Add custom scripts for test-case registration to the module path
if (catch2_POPULATED)
  list(APPEND CMAKE_MODULE_PATH "${catch2_SOURCE_DIR}/contrib")
endif()

# ------------------------------------------------------------------------------
# Spdlog is used for the logging functionality (internally uses the fmt lib)
# ------------------------------------------------------------------------------

set(SPDLOG_BUILD_SHARED ON CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_EXAMPLE OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_EXAMPLE_HO OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_TESTS_HO OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_BENCH OFF CACHE BOOL "" FORCE)

loco_find_or_fetch_dependency(
  USE_SYSTEM_PACKAGE FALSE
  PACKAGE_NAME spdlog
  LIBRARY_NAME spdlog
  GIT_REPO https://github.com/gabime/spdlog.git
  GIT_TAG ${UTILS_DEP_VERSION_spdlog}
  GIT_PROGRESS FALSE
  GIT_SHALLOW TRUE
  TARGETS spdlog::spdlog
  EXCLUDE_FROM_ALL)

# ------------------------------------------------------------------------------
# Pybind11 is used for generating Python bindings for this project's C++ API.
# Notice that we're using a forked version in which usage of unique-ptr is
# allowed, as we use this functionality in some other projects
# ------------------------------------------------------------------------------

set(PYBIND11_TEST OFF CACHE BOOL "" FORCE)

loco_find_or_fetch_dependency(
  USE_SYSTEM_PACKAGE FALSE
  PACKAGE_NAME pybind11
  LIBRARY_NAME pybind11
  GIT_REPO https://github.com/pybind/pybind11.git
  GIT_TAG ${UTILS_DEP_VERSION_pybind11}
  GIT_PROGRESS FALSE
  GIT_SHALLOW TRUE
  TARGETS pybind11::headers
  EXCLUDE_FROM_ALL)

# ------------------------------------------------------------------------------
# Tinyobjloader is used for loading and parsing meshes in .obj file format
# ------------------------------------------------------------------------------

set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)

loco_find_or_fetch_dependency(
  USE_SYSTEM_PACKAGE FALSE
  PACKAGE_NAME tinyobjloader
  LIBRARY_NAME tinyobjloader
  GIT_REPO https://github.com/tinyobjloader/tinyobjloader.git
  GIT_TAG ${UTILS_DEP_VERSION_tinyobjloader}
  GIT_PROGRESS FALSE
  GIT_SHALLOW FALSE
  TARGETS tinyobjloader
  EXCLUDE_FROM_ALL)


# ------------------------------------------------------------------------------
# Math3d is used as math library (defines vectors, matrices, and operations
# that could be used on these types). The API is similar to Eigen's
# ------------------------------------------------------------------------------

set(MATH_BUILD_PYTHON_BINDINGS OFF CACHE BOOL "" FORCE)
set(MATH_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(MATH_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(MATH_BUILD_DOCS OFF CACHE BOOL "" FORCE)

loco_find_or_fetch_dependency(
  USE_SYSTEM_PACKAGE FALSE
  LIBRARY_NAME math
  GIT_REPO https://github.com/wpumacay/math.git
  GIT_TAG ${UTILS_DEP_VERSION_math}
  GIT_PROGRESS FALSE
  GIT_SHALLOW FALSE
  TARGETS math::math
  EXCLUDE_FROM_ALL)

# cmake-format: on
