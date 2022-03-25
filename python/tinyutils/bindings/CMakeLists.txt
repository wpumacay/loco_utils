# ~~~
# CMake configuration rules used to generate Python Bindings via Pybind11
# ~~~

if(NOT TINYUTILS_BUILD_PYTHON_BINDINGS)
  loco_message("Bindings are not required for the current build")
  return()
endif()

# Create target for our python bindings (standalone MODULE)
pybind11_add_module(
  tinyutils
  MODULE
  "${CMAKE_CURRENT_SOURCE_DIR}/src/logging_py.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/path_handling_py.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/timing_py.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/perlin_noise_py.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/profiling_py.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/bindings_py.cpp")
target_include_directories(tinyutils
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(tinyutils PRIVATE TinyUtilsCpp)
