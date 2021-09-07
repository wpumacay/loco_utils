cmake_minimum_required(VERSION 3.15..3.19)

project(
  TinyUtils
  VERSION 0.1.0
  DESCRIPTION "A library of common functionality used alongside my projects"
  HOMEPAGE_URL "https://github.com/wpumacay/tiny_utils"
  LANGUAGES CXX)

# Include some helper functions that we will need later
include(${PROJECT_SOURCE_DIR}/cmake/Common.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/Utils.cmake)

# Include some internal tools we might need to use
include(FetchContent)
include(GNUInstallDirs)

# Check if we're currently working as the root project. If so, configure some
# common settings that we might need later during configuration
tuInitializeProject()

option(TINYUTILS_BUILD_PYTHON_BINDINGS "Build bindings (requires Pybind11)" ON)
option(TINYUTILS_BUILD_EXAMPLES "Build C++ examples ON")
option(TINYUTILS_BUILD_DOCS "Build documentation (requires Doxygen)" OFF)

# Add some external dependencies we need to link against
add_subdirectory(third_party)

add_library(
  TinyUtilsCpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/tinyutils/common.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/tinyutils/logging.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/tinyutils/timing.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/tinyutils/profiling.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/tinyutils/path_handling.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/tinyutils/perlin_noise.cpp)
target_include_directories(TinyUtilsCpp
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(TinyUtilsCpp spdlog::spdlog)
target_compile_features(TinyUtilsCpp PUBLIC cxx_std_11)

# ~~~
# add_subdirectory(examples)
# add_subdirectory(python/tinyutils/bindings)
# ~~~

tuPrintSummary()
