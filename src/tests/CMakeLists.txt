
set( TARGET_VERSION ${${CMAKE_PROJECT_NAME}_VERSION} )
set( TARGET_EXPORT ${CMAKE_PROJECT_NAME}Targets )

include_directories( ${CMAKE_SOURCE_DIR}/src )
include_directories( ${POCO_INCLUDE_DIR} )

if( TARGET leaf_wrapper_weps )
    set( TARGET_NAME weps_test )
    set( TARGET_CATEGORY Test )
    set( TARGET_LANGUAGE CXX )

    add_executable( ${TARGET_NAME} ${TARGET_NAME}.cxx ${TARGET_NAME}.h )
    target_link_libraries( ${TARGET_NAME}
        leaf_util
        leaf_open
        leaf_wrapper_java
        leaf_wrapper_weps
        ${Boost_LIBRARIES}
        ${POCO_LIBRARIES} )

    include( ModuleInstall )
endif()

