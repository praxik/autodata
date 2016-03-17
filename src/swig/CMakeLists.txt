
include( CheckSymbolExists )
include( ${SWIG_USE_FILE} )

add_definitions( -DSWIG )

#Check if uint64_t is the same as unsigned long
try_compile( UINT64T_SAME_AS_ULONG
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_MODULE_PATH}/same_uint64t_ulong.cxx )
#When "-DSWIGWORDSIZE64" is defined SWIG uses unsigned long for uint64_t types
if( ${UINT64T_SAME_AS_ULONG} )
    set( CMAKE_SWIG_GLOBAL_FLAGS "-DSWIGWORDSIZE64" )
endif()

set( SWIG_DIR ${CMAKE_SOURCE_DIR}/src/swig )

#set( TARGET_VERSION ${LEAF_VERSION} )
set( TARGET_EXPORT ${CMAKE_PROJECT_NAME}Targets )
set( TARGET_CATEGORY Swig )
set( PRIVATE_HEADERS "" )
set( PUBLIC_HEADERS "" )
set( PRIVATE_SOURCES "" )

set( MODULE_FILE module.i )

include_directories( ${CMAKE_SOURCE_DIR}/src )
include_directories( ${SWIG_DIR} )
if( RUBY_FOUND )
    include_directories( ${RUBY_INCLUDE_PATH} )
endif()

add_subdirectory( std )
add_subdirectory( Poco )
add_subdirectory( autodata )
