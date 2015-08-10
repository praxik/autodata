
set( INCDIR_NAME "swig/autodata/util" )
set_source_files_properties( ${INTERFACE_FILE} PROPERTIES CPLUSPLUS ON )

if( CSHARP_FOUND )
    set( TARGET_NAME autodata_util_csharp )
    set( SWIG_LANGUAGE CSHARP )

    set( CMAKE_SWIG_FLAGS -namespace autodata.util -dllimport ${TARGET_NAME} )
    set( CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/csharp )
    set( SWIG_MODULE_${TARGET_NAME}_EXTRA_DEPS
        std_csharp
        Poco_csharp
        Poco_Data_csharp
        Poco_Dynamic_csharp )
    swig_add_module( ${TARGET_NAME} csharp ${INTERFACE_FILE} )
    swig_link_libraries( ${TARGET_NAME}
        autodata
        ${Boost_LIBRARIES}
        ${POCO_LIBRARIES} )

    include( ModuleInstall )
endif()

if( RUBY_FOUND )
    set( TARGET_NAME autodata_util_ruby )
    set( SWIG_LANGUAGE RUBY )

    set( CMAKE_SWIG_FLAGS -c++ )
    set( CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/ruby )
    set( SWIG_MODULE_${TARGET_NAME}_EXTRA_DEPS
        std_ruby
        Poco_ruby
        Poco_Data_ruby
        Poco_Dynamic_ruby )
    swig_add_module( ${TARGET_NAME} ruby ${INTERFACE_FILE} )
    swig_link_libraries( ${TARGET_NAME}
        autodata
        ${Boost_LIBRARIES}
        ${POCO_LIBRARIES}
        ${RUBY_LIBRARY} )

    include( ModuleInstall )
endif()
