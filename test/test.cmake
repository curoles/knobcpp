# Add executable called "test_knobcpp".
add_executable (test_knob test/test.cpp)


# Now we can do `make test`
enable_testing()

add_test(NAME test_knob
    #[CONFIGURATIONS [Debug|Release|...]]
    #[WORKING_DIRECTORY dir]
    COMMAND test_knob
)

