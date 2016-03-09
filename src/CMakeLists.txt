
add_subdirectory( autodata )
add_subdirectory( cppiter )
add_subdirectory( cppitertools )
add_subdirectory( cpplinq )
if( SWIG_FOUND )
    add_subdirectory( swig )
endif()
add_subdirectory( tests )
