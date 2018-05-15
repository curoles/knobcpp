add_executable (test_static_knob test/test_static_knob.cpp)
add_executable (test_runtime_knob test/test_runtime_knob.cpp)
add_executable (test_group test/test_group.cpp)
add_executable (test_program_options test/test_program_options.cpp)


# After enablig testing we can do `make test`
enable_testing()

add_test(NAME test_static_knob
    #[CONFIGURATIONS [Debug|Release|...]]
    #[WORKING_DIRECTORY dir]
    COMMAND test_static_knob
)

add_test(NAME test_runtime_knob
    COMMAND test_runtime_knob
)

add_test(NAME test_group
    COMMAND test_group
)

add_test(NAME test_program_options
    COMMAND test_program_options
)

