
set( TARGET_VERSION ${${CMAKE_PROJECT_NAME}_VERSION} )
set( TARGET_EXPORT ${CMAKE_PROJECT_NAME}Targets )

include_directories( ${CMAKE_SOURCE_DIR}/src )
include_directories( ${POCO_INCLUDE_DIR} )

set( TARGET_NAME test )
set( TARGET_CATEGORY Test )
set( TARGET_LANGUAGE CXX )

add_executable( ${TARGET_NAME} ${TARGET_NAME}.cxx )
target_link_libraries( ${TARGET_NAME}
    autodata
    ${POCO_LIBRARIES} )

include( ModuleInstall )

