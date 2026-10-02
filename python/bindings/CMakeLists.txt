include_directories(${Python3_INCLUDE_DIRS})

get_target_property(PYTHON_COMMON_SOURCE_DIR pyariadne-python-common SOURCE_DIR)

add_library(pyariadne-paradigm-submodule-obj OBJECT
    paradigm_submodule.cpp
)
target_include_directories(pyariadne-paradigm-submodule-obj PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${PYTHON_COMMON_SOURCE_DIR}/include"
)
target_link_libraries(pyariadne-paradigm-submodule-obj PRIVATE
    pyariadne-python-common
    pybind11::module
    paradigm
)
set_target_properties(pyariadne-paradigm-submodule-obj PROPERTIES POSITION_INDEPENDENT_CODE ON)

add_library(pyariadne-paradigm INTERFACE)
target_sources(pyariadne-paradigm INTERFACE
    $<TARGET_OBJECTS:pyariadne-paradigm-submodule-obj>
)

if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    add_library(pyariadne-paradigm-module-obj OBJECT
        paradigm_module.cpp
    )
    target_include_directories(pyariadne-paradigm-module-obj PRIVATE
        "${PYTHON_COMMON_SOURCE_DIR}/include"
    )
    target_link_libraries(pyariadne-paradigm-module-obj PRIVATE
        pyariadne-python-common
        pybind11::module
    )
    set_target_properties(pyariadne-paradigm-module-obj PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()
