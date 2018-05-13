# CMakeLists files in this project can refer to the root source directory
# of the project as ${KNOBCPP_SOURCE_DIR} and to the root binary directory
# of the project as ${KNOBCPP_BINARY_DIR}.
cmake_minimum_required (VERSION 3.10)
project (KNOBCPP)

# Install inside the build directory.
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install)

# No external packages so far.
# include(${CMAKE_CURRENT_SOURCE_DIR}/external_packages.cmake)

# Recurse into the "Hello" and "Demo" subdirectories. This does not actually
# cause another cmake executable to run. The same process will walk through
# the project's entire directory structure.
# add_subdirectory (hello)
# add_subdirectory (demo)


# `knobcpp` requires C++17.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

install(FILES knob.h
    DESTINATION include/knobcpp
)

include(test/test.cmake)
include(doc/doc.cmake)

# ------------------------- Begin Generic CMake Variable Logging ------------------
# https://gitlab.kitware.com/cmake/community/wikis/doc/cmake/useful_variables/Logging-Useful-Variables

# the compiler used for C files
MESSAGE( STATUS "CMAKE_C_COMPILER: " ${CMAKE_C_COMPILER} )

# the compiler used for C++ files
MESSAGE( STATUS "CMAKE_CXX_COMPILER: " ${CMAKE_CXX_COMPILER} )

MESSAGE( STATUS "CMAKE_INSTALL_PREFIX: " ${CMAKE_INSTALL_PREFIX} )

