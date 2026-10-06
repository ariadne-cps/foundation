cmake_minimum_required(VERSION 3.15)

project(Foundation VERSION 1.0)

option(WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)
option(BUILD_PYTHON_BINDINGS "Build Python bindings" ON)

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/submodules/configuration/cmake/ProjectOptions.cmake")
    message(FATAL_ERROR
        "Git submodules are not initialized. "
        "Run 'git submodule update --init --recursive' before configuring the project.")
endif()

include("${CMAKE_CURRENT_SOURCE_DIR}/submodules/configuration/cmake/ProjectOptions.cmake")

require_initialized_submodules()

setup_project()

if(EXISTS "${PROJECT_SOURCE_DIR}/.git")
    require_same_dependency_commit(Configuration
        REFERENCES
            "${PROJECT_SOURCE_DIR}|submodules/configuration"
            "${PROJECT_SOURCE_DIR}/submodules/utility|submodules/configuration"
    )
endif()

include_directories(${PROJECT_SOURCE_DIR}/include)

if(NOT TARGET foundation)
    add_subdirectory(submodules)

    add_library(foundation INTERFACE)
    target_include_directories(foundation INTERFACE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/ariadne-foundation>
    )
    target_link_libraries(foundation INTERFACE utility)

    add_subdirectory(src)
    target_link_libraries(FOUNDATION_SRC PRIVATE foundation)
endif()

ariadne_register_public_headers(
    TARGET foundation
    DIRECTORIES "${PROJECT_SOURCE_DIR}/include/foundation"
)

setup_project_library(
    TARGET ariadne-foundation
    OBJECTS
        FOUNDATION_SRC
        UTILITY_SRC
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
