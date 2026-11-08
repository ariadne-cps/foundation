cmake_minimum_required(VERSION 3.15)

project(Foundation VERSION 1.0)

option(WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)
option(BUILD_PYTHON_BINDINGS "Build Python bindings" ON)

if(NOT TARGET ariadne-build)
    find_package(Git REQUIRED)
    # Keep initial Build checkout messages in English.
    execute_process(
        COMMAND "${CMAKE_COMMAND}" -E env "LC_ALL=C"
                "${GIT_EXECUTABLE}" -C "${CMAKE_CURRENT_SOURCE_DIR}"
                submodule update --init -- ss/bui
        RESULT_VARIABLE _result)
    if(_result)
        message(FATAL_ERROR "Cannot initialize build submodule")
    endif()
    include("${CMAKE_CURRENT_SOURCE_DIR}/ss/bui/cmake/ProjectOptions.cmake")
endif()

if(EXISTS "${PROJECT_SOURCE_DIR}/.git")
    require_same_dependency_commit(Build CHAINS
        "build"
        "utility|build"
        "testing|build"
        "utility|testing|build")
    target_link_libraries(foundation INTERFACE utility)

    add_subdirectory(src)
    target_link_libraries(FOUNDATION_SRC PRIVATE foundation)
endif()

ariadne_register_public_headers(
    TARGET foundation
    DIRECTORIES "${PROJECT_SOURCE_DIR}/include/foundation"
    FILES "${PROJECT_SOURCE_DIR}/include/ariadne-foundation.hpp"
)

setup_project_library(
    TARGET ariadne-foundation
    OBJECTS FOUNDATION_SRC
    LINK_LIBRARIES foundation
)

setup_standalone_project_tests(
    TARGET ariadne-foundation
    TEST_DIRECTORY tests
    TEST_TARGET tests
    SOURCES
        "${PROJECT_SOURCE_DIR}/include"
        "${PROJECT_SOURCE_DIR}/src"
)

if(BUILD_PYTHON_BINDINGS)
    add_subdirectory(python)
endif()

if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    ariadne_install_dependency_bundle(
        TARGET foundation
        DESTINATION include/ariadne-foundation
    )
endif()
