include_directories(${Python3_INCLUDE_DIRS})

get_target_property(PYTHON_COMMON_SOURCE_DIR pyariadne-python-common SOURCE_DIR)

add_library(pyariadne-foundation-submodule-obj OBJECT
    foundation_submodule.cpp
)
target_include_directories(pyariadne-foundation-submodule-obj PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${PYTHON_COMMON_SOURCE_DIR}/include"
)
target_link_libraries(pyariadne-foundation-submodule-obj PRIVATE
    pyariadne-python-common
    pybind11::module
    foundation
)
set_target_properties(pyariadne-foundation-submodule-obj PROPERTIES POSITION_INDEPENDENT_CODE ON)

add_library(pyariadne-foundation INTERFACE)
target_sources(pyariadne-foundation INTERFACE
    $<TARGET_OBJECTS:pyariadne-foundation-submodule-obj>
)
target_include_directories(pyariadne-foundation INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    add_library(pyariadne-foundation-module-obj OBJECT
        foundation_module.cpp
    )
    target_include_directories(pyariadne-foundation-module-obj PRIVATE
        "${PYTHON_COMMON_SOURCE_DIR}/include"
    )
    target_link_libraries(pyariadne-foundation-module-obj PRIVATE
        pyariadne-python-common
        pybind11::module
    )
    set_target_properties(pyariadne-foundation-module-obj PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()
