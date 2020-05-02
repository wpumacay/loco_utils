cmake_minimum_required( VERSION 3.0.0 FATAL_ERROR )

project( TinyUtils )

set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fPIC" )

# In case nobody set the project type, set it (parent project might have set it for all)
if ( NOT CMAKE_BUILD_TYPE )
    set( CMAKE_BUILD_TYPE Debug )
endif()

# Compile everything as shared libraries
set( BUILD_SHARED_LIBS ON CACHE BOOL "Build libraries as shared" )

# Outputs of the build should be sent accordingly
if ( NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY )
    message( "TINYUTILS >>> Sending generated libs to OWN build directory" )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/libs" )
else()
    message( "TINYUTILS >>> Sending generated libs to EXTERNAL build directory" )
endif()
message( "TINYUTILS >>> LIBRARY_OUT_DIR: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}" )

if( CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( "TINYUTILS >>> Configuring as MASTER project" )
    set( TINYUTILS_IS_MASTER_PROJECT ON )
else()
    message( "TINYUTILS >>> Configuring as CHILD project" )
    set( TINYUTILS_IS_MASTER_PROJECT OFF )
endif()

option( TINYUTILS_BUILD_EXAMPLES "Build c/c++ examples" ON )
option( TINYUTILS_BUILD_PYTHON_BINDINGS "Build python bindings" ON )

# Add external dependencies only if master project. If child project, root adds the dependencies
if ( TINYUTILS_IS_MASTER_PROJECT )
    add_subdirectory( ext )
endif()

set( TINYUTILS_INCLUDES_DIR
     "${CMAKE_CURRENT_SOURCE_DIR}/include"
     "${CMAKE_CURRENT_SOURCE_DIR}/ext/spdlog/include" )
set( TINYUTILS_SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/tinyutils/common.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/tinyutils/logging.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/tinyutils/profiling.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/tinyutils/path_handling.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/tinyutils/perlin_noise.cpp" )

add_library( tinyutils_cpp_lib SHARED ${TINYUTILS_SRCS} )
target_include_directories( tinyutils_cpp_lib PRIVATE
                            ${TINYUTILS_INCLUDES_DIR} )
target_link_libraries( tinyutils_cpp_lib
                       spdlog
                       dl )

if ( TINYUTILS_BUILD_PYTHON_BINDINGS )
    add_subdirectory( python/tinyutils/bindings )
endif()

if ( TINYUTILS_BUILD_EXAMPLES )
    add_subdirectory( examples )
endif()

message( "***************************************************************************************" )
message( "TINYUTILS >>> BUILD-OPTIONS SUMMARY:" )
message( "CMAKE_BUILD_TYPE                      : ${CMAKE_BUILD_TYPE}" )
message( "CMAKE_LIBRARY_OUTPUT_DIRECTORY        : ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}" )
message( "CMAKE_BUILD_RPATH                     : ${CMAKE_BUILD_RPATH}" )
message( "CMAKE_CXX_FLAGS                       : ${CMAKE_CXX_FLAGS}" )
message( "TINYUTILS_IS_MASTER_PROJECT           : ${TINYUTILS_IS_MASTER_PROJECT}" )
message( "TINYUTILS_BUILD_EXAMPLES              : ${TINYUTILS_BUILD_EXAMPLES}" )
message( "TINYUTILS_BUILD_PYTHON_BINDINGS       : ${TINYUTILS_BUILD_PYTHON_BINDINGS}" )
message( "***************************************************************************************" )