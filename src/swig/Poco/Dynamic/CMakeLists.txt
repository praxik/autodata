
file( GLOB INTERFACE_FILES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.i" )

set( INCDIR_NAME "swig/Poco/Dynamic" )
set_source_files_properties( ${MODULE_FILE} PROPERTIES CPLUSPLUS ON )

if( CSHARP_FOUND )
    set( TARGET_NAME Poco_Dynamic_csharp )
    set( SWIG_LANGUAGE CSHARP )

    set( CMAKE_SWIG_FLAGS ${CMAKE_SWIG_GLOBAL_FLAGS} -namespace Poco.Dynamic -dllimport ${TARGET_NAME} )
    set( CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/csharp )
    set( SWIG_MODULE_${TARGET_NAME}_EXTRA_DEPS
        std_csharp
        Poco_csharp )
    swig_add_module( ${TARGET_NAME} csharp ${MODULE_FILE} )
    swig_link_libraries( ${TARGET_NAME}
        ${POCO_LIBRARIES} )

    include( ModuleInstall )
endif()

if( PYTHONLIBS_FOUND )
    set( TARGET_NAME Poco_Dynamic_python )
    set( SWIG_LANGUAGE PYTHON )

    set( CMAKE_SWIG_FLAGS ${CMAKE_SWIG_GLOBAL_FLAGS} )
    set( CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/python )
    set( SWIG_MODULE_${TARGET_NAME}_EXTRA_DEPS
        _std_python
        _Poco_python )
    swig_add_module( ${TARGET_NAME} python ${MODULE_FILE} )
    swig_link_libraries( ${TARGET_NAME}
        ${POCO_LIBRARIES}
        ${PYTHON_LIBRARIES} )

    include( ModuleInstall )
endif()

if( RUBY_FOUND )
    set( TARGET_NAME Poco_Dynamic_ruby )
    set( SWIG_LANGUAGE RUBY )

    set( CMAKE_SWIG_FLAGS ${CMAKE_SWIG_GLOBAL_FLAGS} -c++ )
    set( CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/ruby )
    set( SWIG_MODULE_${TARGET_NAME}_EXTRA_DEPS
        std_ruby
        Poco_ruby )
    swig_add_module( ${TARGET_NAME} ruby ${MODULE_FILE} )
    swig_link_libraries( ${TARGET_NAME}
        ${POCO_LIBRARIES}
        ${RUBY_LIBRARY} )

    include( ModuleInstall )
endif()
