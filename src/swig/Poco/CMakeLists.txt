
file( GLOB INTERFACE_FILES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.i" )

include_directories( ${POCO_INCLUDE_DIR} )

set( INCDIR_NAME "swig/Poco" )
set_source_files_properties( ${MODULE_FILE} PROPERTIES CPLUSPLUS ON )

if( CSHARP_FOUND )
    set( TARGET_NAME Poco_csharp )
    set( SWIG_LANGUAGE CSHARP )

    set( CMAKE_SWIG_FLAGS ${CMAKE_SWIG_GLOBAL_FLAGS} -namespace Poco -dllimport ${TARGET_NAME} )
    set( CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/csharp )
    set( SWIG_MODULE_${TARGET_NAME}_EXTRA_DEPS
        std_csharp )
    swig_add_module( ${TARGET_NAME} csharp ${MODULE_FILE} )
    swig_link_libraries( ${TARGET_NAME}
        ${POCO_LIBRARIES} )

    include( ModuleInstall )
endif()

if( PYTHONLIBS_FOUND )
    set( TARGET_NAME Poco_python )
    set( SWIG_LANGUAGE PYTHON )

    set( CMAKE_SWIG_FLAGS ${CMAKE_SWIG_GLOBAL_FLAGS} -interface ${TARGET_NAME} )
    set( CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/python )
    set( SWIG_MODULE_${TARGET_NAME}_EXTRA_DEPS
        _std_python )
    swig_add_module( ${TARGET_NAME} python ${MODULE_FILE} )
    swig_link_libraries( ${TARGET_NAME}
        ${POCO_LIBRARIES}
        ${PYTHON_LIBRARIES} )

    include( ModuleInstall )
endif()

if( RUBY_FOUND )
    set( TARGET_NAME Poco_ruby )
    set( SWIG_LANGUAGE RUBY )

    set( CMAKE_SWIG_FLAGS ${CMAKE_SWIG_GLOBAL_FLAGS} -c++ )
    set( CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/ruby )
    set( SWIG_MODULE_${TARGET_NAME}_EXTRA_DEPS
        std_ruby )
    swig_add_module( ${TARGET_NAME} ruby ${MODULE_FILE} )
    swig_link_libraries( ${TARGET_NAME}
        ${POCO_LIBRARIES}
        ${RUBY_LIBRARY} )

    include( ModuleInstall )
endif()

add_subdirectory( Dynamic )
add_subdirectory( Data )
