
if( WIN32 )
    cmake_minimum_required( VERSION 3.0.0 )
else()
    cmake_minimum_required( VERSION 2.8.8 )
endif()

project( AUTODATA )

set( ${CMAKE_PROJECT_NAME}_MAJOR_VERSION 0 )
set( ${CMAKE_PROJECT_NAME}_MINOR_VERSION 1 )
set( ${CMAKE_PROJECT_NAME}_PATCH_VERSION 0 )
set( v ${CMAKE_PROJECT_NAME}_VERSION )
set( ${v} ${${CMAKE_PROJECT_NAME}_MAJOR_VERSION} )
set( ${v} ${${v}}.${${CMAKE_PROJECT_NAME}_MINOR_VERSION} )
set( ${v} ${${v}}.${${CMAKE_PROJECT_NAME}_PATCH_VERSION} )
set( v ${${v}} )
message( STATUS "Configuring ${CMAKE_PROJECT_NAME}: v${v}\n" )

set( INSTALL_LIBDIR lib )
set( INSTALL_BINDIR bin )
set( INSTALL_INCDIR include )
set( INSTALL_SHRDIR share )

if( WIN32 )
    string( REGEX REPLACE "(^| )[/-]D *NDEBUG($| )" " "
        CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}" )
    string( REGEX REPLACE "(^| )[/-]D *NDEBUG($| )" " "
        CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}" )

    if( MSVC )
        if( ${CMAKE_GENERATOR} MATCHES "Win64" )
            set( VC_PLATFORM "x64" )
        else()
            set( VC_PLATFORM "Win32" )
        endif()
        message( STATUS "VC++ Platform: ${VC_PLATFORM}\n" )

        if( MSVC11 )
            #https://connect.microsoft.com/VisualStudio/feedback/details/737019
            add_definitions( -D_VARIADIC_MAX=10 )
        endif()
    endif()
endif()

#Set a default build type if none was specified
if( NOT WIN32 AND NOT CMAKE_BUILD_TYPE )
    set( CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Set build type" FORCE )
endif()

#Update CMake module path
set( CMAKE_MODULE_PATH
    "${PROJECT_SOURCE_DIR}/CMakeModules"
    ${CMAKE_MODULE_PATH} )

#Boost
if( WIN32 )
    add_definitions( -DBOOST_ALL_DYN_LINK )
endif()
set( BOOST_USE_STATIC_LIBS OFF )
set( BOOST_USE_MULTITHREADED ON )
set( BOOST_USE_STATIC_RUNTIME OFF )
set( BOOST_ROOT "Please specify"
    CACHE PATH "Root directory where Boost is installed" )
find_package( Boost 1.53.0 REQUIRED COMPONENTS
    date_time filesystem program_options regex system thread )

#CSharp
if( WIN32 )
    set( CSHARP_DOTNET_VERSION v4.0.30319 )
    set( CSHARP_DOTNET_VERSION ${CSHARP_DOTNET_VERSION} CACHE STRING "" FORCE )
    find_package( CSharp 4.0 )
endif()

#POCO
if( POCO_STATIC )
    add_definitions( -DPOCO_STATIC )
endif()
set( POCO_ROOT "Please specify"
    CACHE PATH "Root directory where POCO is installed" )
find_package( POCO 1.5.3 REQUIRED COMPONENTS
    Data DataSQLite Foundation JSON Net Util XML
    OPTIONAL_COMPONENTS DataMySQL DataODBC )
set( POCO_MYSQL_API_DEFINE "undef" )
set( POCO_ODBC_API_DEFINE "undef" )

#Python
find_package( PythonLibs )

#Ruby
find_package( Ruby 2.2 )

#Swig
find_package( SWIG )

foreach( config IN LISTS CMAKE_CONFIGURATION_TYPES ITEMS ${CMAKE_BUILD_TYPE} )
    string( TOUPPER ${config} config )
    if( config MATCHES "DEBUG" )
        if( POCO_DataMySQL_LIBRARY_DEBUG )
            #set( CMAKE_CXX_FLAGS_DEBUG
            #    "-DPOCO_MYSQL_API ${CMAKE_CXX_FLAGS_DEBUG}" )
            set( POCO_MYSQL_API_DEFINE "define" )
        endif()
        if( POCO_DataODBC_LIBRARY_DEBUG )
            #set( CMAKE_CXX_FLAGS_DEBUG
            #    "-DPOCO_ODBC_API ${CMAKE_CXX_FLAGS_DEBUG}" )
            set( POCO_ODBC_API_DEFINE "define" )
        endif()
    else()
        if( POCO_DataMySQL_LIBRARY )
            #set( CMAKE_CXX_FLAGS_${config}
            #    "-DPOCO_MYSQL_API ${CMAKE_CXX_FLAGS_${config}}" )
            set( POCO_MYSQL_API_DEFINE "define" )
        endif()
        if( POCO_DataODBC_LIBRARY )
            #set( CMAKE_CXX_FLAGS_${config}
            #    "-DPOCO_ODBC_API ${CMAKE_CXX_FLAGS_${config}}" )
            set( POCO_ODBC_API_DEFINE "define" )
        endif()
    endif()
endforeach()
configure_file( ${PROJECT_SOURCE_DIR}/src/autodata/AutoDataConfig.h.in
    ${PROJECT_SOURCE_DIR}/src/autodata/AutoDataConfig.h @ONLY )

add_subdirectory( src )

set( EXPORT_LIBRARY_TARGETS
    autodata )

set( CONFIGURE_VARS
    INSTALL_LIBDIR )

include( InstallAutoDiscoveryTools )
