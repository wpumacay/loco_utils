message( "TINYUTILS::Examples >>> configuring examples for the usage of the library" )

file( GLOB ExampleFiles *.cpp )

# Define test builder helper for the examples
function( FcnBuildExample paramSources paramExecutableName )
    add_executable( ${paramExecutableName}
                    ${paramSources} )
    target_link_libraries( ${paramExecutableName}
                           tinyutils_cpp_lib )
    target_include_directories( ${paramExecutableName} PRIVATE
                                ${TINYUTILS_INCLUDES_DIR} )
endfunction()

foreach( sampleFile ${ExampleFiles} )
    # remove extension from sample file
    string( REPLACE ".cpp" "" executableLongName ${sampleFile} )
    # remove other path components from file
    get_filename_component( executableName ${executableLongName} NAME )

    FcnBuildExample( ${sampleFile} ${executableName} )

endforeach( sampleFile )