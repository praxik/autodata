
set_source_files_properties( ${MODULE_FILE} PROPERTIES CPLUSPLUS ON )

if( CSHARP_FOUND )
    set( TARGET_NAME autodata_db_csharp )
    set( SWIG_LANGUAGE CSHARP )

    set( CMAKE_SWIG_FLAGS ${CMAKE_SWIG_GLOBAL_FLAGS} -namespace autodata.db -dllimport ${TARGET_NAME} )
    set( CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/csharp )
    set( SWIG_MODULE_${TARGET_NAME}_EXTRA_DEPS
        std_csharp
        Poco_csharp
        Poco_Data_csharp
        Poco_Dynamic_csharp
        autodata_util_csharp )
    swig_add_module( ${TARGET_NAME} csharp ${MODULE_FILE} )
    swig_link_libraries( ${TARGET_NAME}
        autodata
        ${Boost_LIBRARIES}
        ${POCO_LIBRARIES} )

    include( ModuleInstall )
endif()

if( PYTHONLIBS_FOUND )
    set( TARGET_NAME autodata_db_python )
    set( SWIG_LANGUAGE PYTHON )

    set( CMAKE_SWIG_FLAGS ${CMAKE_SWIG_GLOBAL_FLAGS} -interface _${TARGET_NAME} )
    set( CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/python )
    set( SWIG_MODULE_${TARGET_NAME}_EXTRA_DEPS
        _std_python
        _Poco_python
        _Poco_Data_python
        _Poco_Dynamic_python
        _autodata_util_python )
    swig_add_module( ${TARGET_NAME} python ${MODULE_FILE} )
    swig_link_libraries( ${TARGET_NAME}
        autodata
        ${Boost_LIBRARIES}
        ${POCO_LIBRARIES}
        ${PYTHON_LIBRARIES} )

    include( ModuleInstall )
endif()

if( RUBY_FOUND )
    set( TARGET_NAME autodata_db_ruby )
    set( SWIG_LANGUAGE RUBY )

    set( CMAKE_SWIG_FLAGS ${CMAKE_SWIG_GLOBAL_FLAGS} -c++ )
    set( CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/ruby )
    set( SWIG_MODULE_${TARGET_NAME}_EXTRA_DEPS
        std_ruby
        Poco_ruby
        Poco_Data_ruby
        Poco_Dynamic_ruby
        autodata_util_ruby )
    swig_add_module( ${TARGET_NAME} ruby ${MODULE_FILE} )
    swig_link_libraries( ${TARGET_NAME}
        autodata
        ${Boost_LIBRARIES}
        ${POCO_LIBRARIES}
        ${RUBY_LIBRARY} )

    include( ModuleInstall )
endif()
