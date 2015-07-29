
set( TARGET_VERSION ${${CMAKE_PROJECT_NAME}_VERSION} )
set( TARGET_EXPORT ${CMAKE_PROJECT_NAME}Targets )

include_directories( ${CMAKE_SOURCE_DIR}/src )
include_directories( ${Boost_INCLUDE_DIRS} )
include_directories( ${POCO_INCLUDE_DIR} )

if( TARGET autodata )
    #autodata_test
    set( TARGET_NAME autodata_test )
    set( TARGET_CATEGORY Test )
    set( TARGET_LANGUAGE CXX )

    add_executable( ${TARGET_NAME} ${TARGET_NAME}.cxx )
    target_link_libraries( ${TARGET_NAME}
        autodata
        ${Boost_LIBRARIES}
        ${POCO_LIBRARIES} )

    include( ModuleInstall )

    #agg_test
    set( TARGET_NAME agg_test )
    set( TARGET_CATEGORY Test )
    set( TARGET_LANGUAGE CXX )

    add_executable( ${TARGET_NAME} ${TARGET_NAME}.cxx )
    target_link_libraries( ${TARGET_NAME}
        autodata
        ${Boost_LIBRARIES}
        ${POCO_LIBRARIES} )

    include( ModuleInstall )
endif()

if( TRUE )
    #cppiter_test
    set( TARGET_NAME cppiter_test )
    set( TARGET_CATEGORY Test )
    set( TARGET_LANGUAGE CXX )

    add_executable( ${TARGET_NAME} ${TARGET_NAME}.cxx )
    target_link_libraries( ${TARGET_NAME}
        ${Boost_LIBRARIES}
        ${POCO_LIBRARIES} )

    include( ModuleInstall )
endif()
