message( "TINYUTILS::Python >>> configuring python-bindings" )

pybind11_add_module( tinyutils
                     src/logging_py.cpp
                     src/path_handling_py.cpp
                     src/timing_py.cpp
                     src/perlin_noise_py.cpp
                     src/profiling_py.cpp
                     src/bindings_py.cpp )
target_link_libraries( tinyutils PRIVATE tinyutils_cpp_lib )
target_include_directories( tinyutils PRIVATE 
                            "${CMAKE_CURRENT_SOURCE_DIR}/include"
                            "${TINYUTILS_INCLUDES_DIR}" )