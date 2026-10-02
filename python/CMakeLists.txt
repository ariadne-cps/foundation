set(Python3_FIND_VIRTUALENV FIRST)
set(Python3_FIND_STRATEGY LOCATION)
set(PYBIND11_FINDPYTHON ON)

find_package(Python3 3.11 COMPONENTS Interpreter Development.Module)

option(PYTHON_ENFORCE_TESTS "Enforce Python tests" OFF)

if(Python3_FOUND)
    message(STATUS "Python executable: ${Python3_EXECUTABLE}")
    message(STATUS "Python version:    ${Python3_VERSION}")
    message(STATUS "Python includes:   ${Python3_INCLUDE_DIRS}")
    message(STATUS "Python SOABI:      ${Python3_SOABI}")

    if(NOT TARGET pybind11::module)
        get_target_property(PYTHON_COMMON_SOURCE_DIR pyariadne-python-common SOURCE_DIR)
        add_subdirectory(
            "${PYTHON_COMMON_SOURCE_DIR}/pybind11"
            "${CMAKE_CURRENT_BINARY_DIR}/pybind11"
        )
    endif()

    add_subdirectory(source)

    if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
        pybind11_add_module(pyariadne-paradigm
            $<TARGET_OBJECTS:pyariadne-paradigm-module-obj>
            $<TARGET_OBJECTS:pyariadne-paradigm-submodule-obj>
            $<TARGET_OBJECTS:UTILITY_SRC>
            $<TARGET_OBJECTS:PARADIGM_SRC>
        )
        set_target_properties(pyariadne-paradigm PROPERTIES OUTPUT_NAME "pyariadne_paradigm")
        target_link_libraries(pyariadne-paradigm PRIVATE paradigm)

        set(PYTHON_BINDINGS_INSTALL_DIR "${Python3_SITEARCH}"
            CACHE STRING "Install destination for Python targets")
        install(TARGETS pyariadne-paradigm DESTINATION "${PYTHON_BINDINGS_INSTALL_DIR}")

        add_subdirectory(test)
        add_dependencies(bindings pyariadne-paradigm)
    endif()
else()
    if(PYTHON_ENFORCE_TESTS)
        message(FATAL_ERROR "Python >= 3.11 with Interpreter and Development.Module components is required when PYTHON_ENFORCE_TESTS=ON.")
    else()
        message(STATUS "Compatible Python development environment not found: Python bindings will not be built.")
    endif()
endif()
