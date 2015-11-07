
set( INCDIR_NAME "swig/std" )
set_source_files_properties( ${INTERFACE_FILE} PROPERTIES CPLUSPLUS ON )

if( CSHARP_FOUND )
    set( TARGET_NAME std_csharp )
    set( SWIG_LANGUAGE CSHARP )

    set( CMAKE_SWIG_FLAGS -namespace std -dllimport ${TARGET_NAME} )
    set( CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/csharp )
    set( SWIG_MODULE_${TARGET_NAME}_EXTRA_DEPS
        ${SWIG_DIR}/common.i )
    swig_add_module( ${TARGET_NAME} csharp ${INTERFACE_FILE} )
    swig_link_libraries( ${TARGET_NAME} )

    include( ModuleInstall )
endif()

if( RUBY_FOUND )
    set( TARGET_NAME std_ruby )
    set( SWIG_LANGUAGE RUBY )

    if( CMAKE_SWIG_GLOBAL_FLAGS )
        set( CMAKE_SWIG_FLAGS "${CMAKE_SWIG_GLOBAL_FLAGS} -c++" )
    else()
        set( CMAKE_SWIG_FLAGS "-c++" )
    endif()
    set( CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/ruby )
    set( SWIG_MODULE_${TARGET_NAME}_EXTRA_DEPS
        ${SWIG_DIR}/common.i )
    swig_add_module( ${TARGET_NAME} ruby ${INTERFACE_FILE} )
    swig_link_libraries(
        ${TARGET_NAME}
        ${RUBY_LIBRARY} )

    include( ModuleInstall )
endif()
