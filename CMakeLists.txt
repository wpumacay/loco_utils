cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

# -------------------------------------
# Get loco_cmake to help us configure our CMake based project
include(FetchContent)
FetchContent_Declare(
  loco_cmake
  GIT_REPOSITORY https://github.com/wpumacay/loco_cmake.git
  GIT_TAG dev
  GIT_PROGRESS TRUE
  GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(loco_cmake)
include(${loco_cmake_SOURCE_DIR}/Index.cmake)

# -------------------------------------
# Define our project :)
project(
  LocoUtils
  VERSION 0.1.2
  DESCRIPTION "A library of common functionality used alongside my projects"
  HOMEPAGE_URL "https://github.com/wpumacay/loco_utils"
  LANGUAGES C CXX)

# -------------------------------------
# Define some options the user can set before|while configuring the project
option(LOCOUTILS_BUILD_PYTHON_BINDINGS "Build bindings (requires Pybind11)" ON)
option(LOCOUTILS_BUILD_EXAMPLES "Build C++ examples" ON)
option(LOCOUTILS_BUILD_DOCS "Build documentation (requires Doxygen)" OFF)

# -------------------------------------
# Initialize the project using our helper modules :D
loco_initialize_project()

# -------------------------------------
# Allow the usage of our helper macros and functions
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# -------------------------------------
# Bring our dependencies accordingly
include(LocoUtilsDependencies)

# cmake-format: off
# -------------------------------------
# Setup the main C++ target `LocoUtilsCpp`
loco_create_target(LocoUtilsCpp STATIC
 SOURCES
   ${CMAKE_CURRENT_SOURCE_DIR}/src/loco/utils/common.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/loco/utils/logging.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/loco/utils/timing.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/loco/utils/profiling.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/loco/utils/path_handling.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/loco/utils/perlin_noise.cpp
 INCLUDE_DIRECTORIES
   ${CMAKE_CURRENT_SOURCE_DIR}/include
 TARGET_DEPENDENCIES
   spdlog::spdlog
 WARNINGS_AS_ERRORS FALSE)
# cmake-format: on

# -------------------------------------
# Expose an alias for the library (CMake namespace convention)
add_library(loco::utils ALIAS LocoUtilsCpp)

# -------------------------------------
# Show the properties of our main target
loco_print_target_properties(LocoUtilsCpp)

# -------------------------------------
# Show some info of the this project
loco_print_project_info()

# -------------------------------------
# Add the set of examples created along this project
add_subdirectory(examples)

# -------------------------------------
# Add the set of Python bindings created for LocoUtilsCpp
# add_subdirectory(python/loco/utils/bindings)

if(LOCOUTILS_BUILD_DOCS)
  # cmake-format: off
  # -------------------------------------
  # Setup documentation for our target
  loco_setup_cppdocs(LocoUtilsCpp
    DOXYGEN_FILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in
    DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/docs/Doxygen
    DOXYGEN_GENERATE_HTML FALSE
    DOXYGEN_GENERATE_LATEX FALSE
    DOXYGEN_GENERATE_XML TRUE
    DOXYGEN_QUIET TRUE
    SPHINX_FILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/conf.py.in
    SPHINX_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/docs/Sphinx
    SPHINX_BREATHE_PROJECT LocoUtilsCpp
    SPHINX_DOXYGEN_XML_OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/docs/Doxygen/xml)
  # cmake-format: on
endif()
