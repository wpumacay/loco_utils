# ~~~
# CMake configuration for examples of this project's usage
# ~~~

if(NOT TINYUTILS_BUILD_EXAMPLES)
  tuMessage("Examples are not required for the current build" LOG_LEVEL TRACE)
  return()
endif()

set(TINYUTILS_EXAMPLE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/example_logger_console.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/example_logger_file.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/example_path_handling.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/example_perlin_noise.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/example_profiler_external_chrome.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/example_smart_pointers_macro.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/example_timing.cpp")

foreach(ex_file IN LISTS TINYUTILS_EXAMPLE_FILES)
  # Get the name of the target for the current example
  get_filename_component(ex_target_name ${ex_file} NAME_WLE)
  # Create the example target for the current example file
  add_executable(${ex_target_name} ${ex_file})
  target_link_libraries(${ex_target_name} PRIVATE TinyUtilsCpp)
endforeach()
