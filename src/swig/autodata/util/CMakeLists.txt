
set_source_files_properties( ${MODULE_FILE} PROPERTIES CPLUSPLUS ON )

if( CSHARP_FOUND )
    set( MODULE_NAME autodata_util )
    set( TARGET_NAME ${MODULE_NAME}_csharp )
    set( SWIG_LANGUAGE CSHARP )

    set( CMAKE_SWIG_FLAGS ${CMAKE_SWIG_GLOBAL_FLAGS} -namespace autodata.util -dllimport ${TARGET_NAME} )
    set( CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/csharp )
    set( SWIG_MODULE_${TARGET_NAME}_EXTRA_DEPS
        std_csharp
        Poco_csharp
        Poco_Data_csharp
        Poco_Dynamic_csharp )
    swig_add_library( ${TARGET_NAME} LANGUAGE csharp SOURCES ${MODULE_FILE} )
    swig_link_libraries( ${TARGET_NAME}
        autodata
        ${Boost_LIBRARIES}
        ${POCO_LIBRARIES} )

    include( ModuleInstall )
endif()

if( PYTHONLIBS_FOUND )
    set( MODULE_NAME autodata_util )
    set( TARGET_NAME ${MODULE_NAME}_python )
    set( SWIG_LANGUAGE PYTHON )

    #Prevent UseSWIG from rebuilding source...
    set_property( SOURCE ${MODULE_FILE} PROPERTY SWIG_MODULE_NAME ${MODULE_NAME} )
    set( CMAKE_SWIG_FLAGS ${CMAKE_SWIG_GLOBAL_FLAGS} -interface _${TARGET_NAME} )
    set( CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/python )
    set( SWIG_MODULE_${TARGET_NAME}_EXTRA_DEPS
        _std_python
        _Poco_python
        _Poco_Data_python
        _Poco_Dynamic_python )
    swig_add_library( ${TARGET_NAME} LANGUAGE python SOURCES ${MODULE_FILE} )
    swig_link_libraries( ${TARGET_NAME}
        autodata
        ${Boost_LIBRARIES}
        ${POCO_LIBRARIES}
        ${PYTHON_LIBRARIES} )

    include( ModuleInstall )
endif()

if( RUBY_FOUND )
    set( MODULE_NAME autodata_util_ruby )
    set( TARGET_NAME ${MODULE_NAME} )
    set( SWIG_LANGUAGE RUBY )

    set( CMAKE_SWIG_FLAGS ${CMAKE_SWIG_GLOBAL_FLAGS} -c++ )
    set( CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/ruby )
    set( SWIG_MODULE_${TARGET_NAME}_EXTRA_DEPS
        std_ruby
        Poco_ruby
        Poco_Data_ruby
        Poco_Dynamic_ruby )
    swig_add_library( ${TARGET_NAME} LANGUAGE ruby SOURCES ${MODULE_FILE} )
    swig_link_libraries( ${TARGET_NAME}
        autodata
        ${Boost_LIBRARIES}
        ${POCO_LIBRARIES}
        ${RUBY_LIBRARY} )

    include( ModuleInstall )
endif()
