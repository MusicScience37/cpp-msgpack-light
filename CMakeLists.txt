cmake_minimum_required(VERSION 3.12)

set(SHORT_PROJECT_NAME "msgpack_light")
set(FULL_PROJECT_NAME "cpp-msgpack-light")
string(TOUPPER ${SHORT_PROJECT_NAME} UPPER_PROJECT_NAME)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(ParseVersion)
parse_version(
    ${CMAKE_CURRENT_SOURCE_DIR}/include/${SHORT_PROJECT_NAME}/version.h
    ${UPPER_PROJECT_NAME})

project(
    ${SHORT_PROJECT_NAME}
    VERSION ${${UPPER_PROJECT_NAME}_VERSION}
    DESCRIPTION "A light library to serialize MessagePack."
    LANGUAGES CXX)
message(STATUS "${FULL_PROJECT_NAME} version ${PROJECT_VERSION}")
message(STATUS "build type: ${CMAKE_BUILD_TYPE}")

string(TOUPPER ${PROJECT_NAME} UPPER_PROJECT_NAME)

set(BUILDNAME
    "${CMAKE_HOST_SYSTEM_NAME}-${CMAKE_CXX_COMPILER_ID}-${CMAKE_CXX_COMPILER_VERSION}-${CMAKE_BUILD_TYPE}"
)
include(CTest)

option(${UPPER_PROJECT_NAME}_BUILD_DOC
       "build documentation of ${FULL_PROJECT_NAME}" OFF)
option(${UPPER_PROJECT_NAME}_TESTING "enable tests of ${FULL_PROJECT_NAME}" OFF)
option(${UPPER_PROJECT_NAME}_ENABLE_INSTALL
       "enable installation of ${FULL_PROJECT_NAME}" ON)

set(CMAKE_CXX_STANDARD
    "17"
    CACHE STRING "C++ standard")

include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY $<1:${CMAKE_BINARY_DIR}/lib>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY $<1:${CMAKE_BINARY_DIR}/lib>)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_BINARY_DIR}/bin>)

if(${UPPER_PROJECT_NAME}_TESTING)
    find_package(Catch2 REQUIRED)
    find_package(msgpack-cxx REQUIRED)
    find_package(celero REQUIRED)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS
    ON
    CACHE BOOL "export compile commands" FORCE)

include(CppWarningFlags)
include(ConfigureSanitizer)
include(ConfigureCcache)
include(poetry)

set(${UPPER_PROJECT_NAME}_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(
    ${PROJECT_NAME}
    INTERFACE $<BUILD_INTERFACE:${${UPPER_PROJECT_NAME}_SOURCE_DIR}/include>
              $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_link_libraries(${PROJECT_NAME}
                      INTERFACE $<BUILD_INTERFACE:${PROJECT_NAME}_cpp_warnings>)
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)

if(${UPPER_PROJECT_NAME}_BUILD_DOC)
    add_subdirectory(doc)
endif()

if(${UPPER_PROJECT_NAME}_TESTING)
    add_subdirectory(tests)
endif()

if(${UPPER_PROJECT_NAME}_ENABLE_INSTALL)
    install(TARGETS ${PROJECT_NAME} EXPORT ${FULL_PROJECT_NAME}-targets)

    install(
        DIRECTORY ${${UPPER_PROJECT_NAME}_SOURCE_DIR}/include/msgpack_light
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING
        PATTERN "*.h")

    install(
        EXPORT ${FULL_PROJECT_NAME}-targets
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${FULL_PROJECT_NAME}
        NAMESPACE cpp_msgpack_light::)

    include(CMakePackageConfigHelpers)
    configure_package_config_file(
        cmake/${FULL_PROJECT_NAME}-config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/${FULL_PROJECT_NAME}-config.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${FULL_PROJECT_NAME})

    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${FULL_PROJECT_NAME}-config.cmake
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${FULL_PROJECT_NAME})
endif()
