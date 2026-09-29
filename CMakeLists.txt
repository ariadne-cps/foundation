cmake_minimum_required(VERSION 3.15)

project(Foundation VERSION 1.0)

option(WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/submodules/configuration/cmake/ProjectOptions.cmake")
    message(FATAL_ERROR
        "Git submodules are not initialized. "
        "Run 'git submodule update --init --recursive' before configuring the project.")
endif()

include("${CMAKE_CURRENT_SOURCE_DIR}/submodules/configuration/cmake/ProjectOptions.cmake")

require_initialized_submodules()

option(THREAD_SANITIZER "Enable ThreadSanitizer" OFF)

setup_project()

if(COVERAGE AND THREAD_SANITIZER)
    message(FATAL_ERROR "COVERAGE and THREAD_SANITIZER cannot be enabled together.")
endif()

if(THREAD_SANITIZER)
    if(WIN32 OR NOT CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
        message(FATAL_ERROR "ThreadSanitizer is supported only with GCC or Clang on Unix-like systems.")
    endif()
    add_compile_options(-fsanitize=thread -fno-omit-frame-pointer -g)
    add_link_options(-fsanitize=thread)
endif()

if(EXISTS "${PROJECT_SOURCE_DIR}/.git")
    require_same_dependency_commit(Configuration
        REFERENCES
            "${PROJECT_SOURCE_DIR}|submodules/configuration"
            "${PROJECT_SOURCE_DIR}/submodules/utility|submodules/configuration"
    )
endif()

include_directories(SYSTEM ${PROJECT_SOURCE_DIR}/include)

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

if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    get_target_property(UTILITY_SOURCE_DIR utility SOURCE_DIR)

    install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/foundation"
        DESTINATION include/ariadne-foundation
    )
    install(DIRECTORY "${UTILITY_SOURCE_DIR}/include/utility"
        DESTINATION include/ariadne-foundation
    )
endif()
