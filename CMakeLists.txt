cmake_minimum_required(VERSION 3.20)
project(HTTPClient VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)

include(cmake/CPM.cmake)

CPMAddPackage(
        NAME HashMap
        GITHUB_REPOSITORY ximtech/HashMap
        GIT_TAG origin/main)

CPMAddPackage(
        NAME Ethernet
        GITHUB_REPOSITORY ximtech/Ethernet
        GIT_TAG origin/main)

CPMAddPackage(
        NAME URLParser
        GITHUB_REPOSITORY ximtech/URLParser
        GIT_TAG origin/main)

set(SOURCE_FILES
        HTTPClient.c
        include/HTTPClient.h)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

target_include_directories(${PROJECT_NAME} PUBLIC
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

target_link_libraries(${PROJECT_NAME} HashMap)
target_link_libraries(${PROJECT_NAME} Ethernet)
target_link_libraries(${PROJECT_NAME} URLParser)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}.h
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Export
        DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/${PROJECT_NAME})

install(EXPORT ${PROJECT_NAME}Export
        FILE ${PROJECT_NAME}-config.cmake
        DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/${PROJECT_NAME})