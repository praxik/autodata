
include( ${SWIG_USE_FILE} )

add_definitions( -DSWIG )

set( SWIG_DIR ${CMAKE_SOURCE_DIR}/src/swig )

#set( TARGET_VERSION ${LEAF_VERSION} )
set( TARGET_EXPORT ${CMAKE_PROJECT_NAME}Targets )
set( TARGET_CATEGORY Swig )
set( PRIVATE_HEADERS "" )
set( PUBLIC_HEADERS "" )
set( PRIVATE_SOURCES "" )

set( INTERFACE_FILE module.i )

include_directories( ${CMAKE_SOURCE_DIR}/src )
include_directories( ${SWIG_DIR} )
if( RUBY_FOUND )
    include_directories( ${RUBY_INCLUDE_PATH} )
endif()

add_subdirectory( std )
add_subdirectory( Poco )
add_subdirectory( autodata )
