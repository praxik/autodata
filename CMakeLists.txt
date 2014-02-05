cmake_minimum_required (VERSION 2.8.8)

add_subdirectory( noTH )
add_subdirectory( storable )


project( json_util_proj )

SET( CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/../../../CMakeModules" )


set( POCO_ROOT "Please specify" CACHE PATH "Root directory where POCO is installed" )
FIND_PACKAGE(POCO 1.5.1 REQUIRED COMPONENTS PocoFoundation PocoUtil PocoData PocoDataSQLite PocoJSON)


INCLUDE_DIRECTORIES(
    ${POCO_INCLUDE_DIR}
)

set( HEADERS AutoData.h AutoDataCRTP.h AutoDataConcept.h ADerivedDataClass.h Persister.h )
set( SOURCES AutoData.cxx ADerivedDataClass.cxx test.cxx )

set( TARGET_NAME json_util )
set( TARGET_VERSION 0.1 )
set( TARGET_EXPORT ${CMAKE_PROJECT_NAME}Targets )
set( TARGET_CATEGORY App )

include_directories( ${POCO_INCLUDE_DIR} )

add_executable( ${TARGET_NAME} ${SOURCES} ${HEADERS} )
target_link_libraries( ${TARGET_NAME}
    ${POCO_LIBRARIES} )

SET(INSTALL_BINDIR bin)

INSTALL(
        TARGETS ${TARGET_NAME}
        RUNTIME DESTINATION ${INSTALL_BINDIR}
    )

install(FILES data.json
    DESTINATION bin
    COMPONENT development)
