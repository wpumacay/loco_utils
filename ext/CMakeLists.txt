message( "TINYUTILS::ext >>> configuring external dependencies" )

# build spdlog as a shared library
set( SPDLOG_BUILD_SHARED ON CACHE BOOL "Build shared library" )
set( SPDLOG_BUILD_EXAMPLE OFF CACHE BOOL "Don't build spdlog examples" )
set( SPDLOG_BUILD_EXAMPLE_HO OFF CACHE BOOL "Don't build spdlog examples (ho-flag)" )
set( SPDLOG_BUILD_TESTS OFF CACHE BOOL "Don't build spdlog tests" )
set( SPDLOG_BUILD_TESTS_HO OFF CACHE BOOL "Don't build spdlog tests (ho-flag)" )
set( SPDLOG_BUILD_BENCH OFF CACHE BOOL "Don't build spdlog benchmarks" )

add_subdirectory( spdlog )
add_subdirectory( pybind11 )