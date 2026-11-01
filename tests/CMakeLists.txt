include(CTest)

set(UNIT_TESTS
    test_logical
    test_paradigm
    test_representation
)

foreach(TEST ${UNIT_TESTS})
    add_executable(${TEST} ${TEST}.cpp)
    target_link_libraries(${TEST} ariadne-foundation foundation ariadne-testing)
    add_test(NAME ${TEST} COMMAND $<TARGET_FILE:${TEST}> --concise)
    if(WIN32)
        set_tests_properties(${TEST} PROPERTIES
            ENVIRONMENT_MODIFICATION
                "PATH=path_list_prepend:$<TARGET_FILE_DIR:ariadne-foundation>"
        )
    endif()
endforeach()

add_custom_target(tests)
add_dependencies(tests ${UNIT_TESTS})
