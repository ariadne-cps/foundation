cmake_minimum_required(VERSION 3.15)

project(Paradigm VERSION 1.0)

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

include_directories(SYSTEM ${PROJECT_SOURCE_DIR}/include)

if(NOT TARGET paradigm)
    add_subdirectory(submodules)

    add_library(paradigm INTERFACE)
    target_include_directories(paradigm INTERFACE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/ariadne-paradigm>
    )
    target_link_libraries(paradigm INTERFACE utility)

    add_subdirectory(src)
    target_link_libraries(PARADIGM_SRC PRIVATE paradigm)
endif()

setup_project_library(
    TARGET ariadne-paradigm
    OBJECTS
        PARADIGM_SRC
        UTILITY_SRC
    LINK_LIBRARIES paradigm
)

setup_standalone_project_tests(
    TARGET ariadne-paradigm
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
    get_target_property(UTILITY_SOURCE_DIR utility SOURCE_DIR)

    install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/paradigm"
        DESTINATION include/ariadne-paradigm
    )
    install(DIRECTORY "${UTILITY_SOURCE_DIR}/include/utility"
        DESTINATION include/ariadne-paradigm
    )
endif()
