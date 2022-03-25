cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

# -------------------------------------
# Get loco_cmake to help us configure our CMake based project
include(FetchContent)
FetchContent_Declare(
  loco_cmake
  GIT_REPOSITORY https://github.com/wpumacay/loco_cmake.git
  GIT_TAG main
  GIT_PROGRESS TRUE
  GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(loco_cmake)
include(${loco_cmake_SOURCE_DIR}/Index.cmake)

# -------------------------------------
# Define our project :)
project(
  TinyUtils
  VERSION 0.1.0
  DESCRIPTION "A library of common functionality used alongside my projects"
  HOMEPAGE_URL "https://github.com/wpumacay/tiny_utils"
  LANGUAGES C CXX)

# -------------------------------------
# Define some options the user can set before|while configuring the project
option(TINYUTILS_BUILD_PYTHON_BINDINGS "Build bindings (requires Pybind11)" ON)
option(TINYUTILS_BUILD_EXAMPLES "Build C++ examples" ON)
option(TINYUTILS_BUILD_DOCS "Build documentation (requires Doxygen)" OFF)

# -------------------------------------
# Initialize the project using our helper modules :D
loco_initialize_project()

# -------------------------------------
# Get the required third-party dependencies
add_subdirectory(third_party)

# cmake-format: off
# -------------------------------------
# Setup the main C++ target `TinyUtilsCpp`
add_library(TinyUtilsCpp)
loco_setup_target(
  TinyUtilsCpp
  SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tinyutils/common.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tinyutils/logging.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tinyutils/timing.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tinyutils/profiling.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tinyutils/path_handling.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tinyutils/perlin_noise.cpp
  INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  TARGET_DEPENDENCIES
    spdlog::spdlog
  WARNINGS_AS_ERRORS FALSE)
# cmake-format: on

# -------------------------------------
# Show the properties of our main target
loco_print_target_properties(TinyUtilsCpp)

# -------------------------------------
# Show some info of the this project
loco_print_project_info()

# -------------------------------------
# Add the set of examples created along this project
add_subdirectory(examples)

# -------------------------------------
# Add the set of Python bindings created for TinyUtilsCpp
add_subdirectory(python/tinyutils/bindings)
