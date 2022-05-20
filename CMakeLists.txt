################################################################################
## Project setup ###############################################################
################################################################################

cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(Arithmetic-Expressions-Parser-Evaluator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Don't use GNU extension (like -std=gnu++11) for portability
set(CMAKE_CXX_EXTENSIONS OFF)

################################################################################
## Fetch External Libraries ####################################################
################################################################################

include(FetchContent)

## Google Test #################################################################
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        e2239ee6043f73722e7aa812a459f54a28552929 # release-1.11.0
)
FetchContent_MakeAvailable(googletest)

################################################################################
## Compile flags ###############################################################
################################################################################

add_compile_options(
        -pedantic
        -pedantic-errors
        -Wall
        -Wextra
        -fstack-protector-strong
        -ftrapv
        -fwrapv
        -Wcast-align
        -Wcast-qual
        -Wconversion
        -Wdouble-promotion
        -Wfloat-equal
        -Wformat=2
        -Wformat-security
        -Wmissing-include-dirs
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Wredundant-decls
        -Wshadow
        -Wsign-conversion
        -Wswitch-enum
        -Wuninitialized
        -Wunused-parameter
)

## Options supported by GNU ####################################################
if (CMAKE_CXX_COMPILER_ID STREQUAL GNU)
    add_compile_options(
            -Walloc-zero
            -Walloca
            -Wduplicated-branches
            -Wduplicated-cond
            -Wformat-signedness
            -Wimplicit-fallthrough=5
            -Wlogical-op
            -Werror
    )
endif ()

if(CMAKE_BUILD_TYPE STREQUAL Debug)
    add_compile_definitions(DEBUG_BUILD)
endif()

################################################################################
## Build options ###############################################################
################################################################################

option(BUILD_TESTS "Build tests" ON)
option(BUILD_DOCUMENTATION "Build documentation" ON)

################################################################################
## Tests #######################################################################
################################################################################

if (BUILD_TESTS)
    enable_testing()
    include(GoogleTest)
    add_subdirectory(tests)
endif ()


################################################################################
## Generate Documentation ######################################################
################################################################################

if (BUILD_DOCUMENTATION)
    find_package(Doxygen)
    if (DOXYGEN_FOUND)
        set(DOXYGEN_IN ${CMAKE_SOURCE_DIR}/doxygen/Doxyfile.in)
        set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/doxygen/Doxyfile)

        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
        message("Building Doxygen")

        # Build documentation at the same time as the project
        add_custom_target(doxygen ALL
                COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                COMMENT "Generating Doxygen"
                VERBATIM)
    else (DOXYGEN_FOUND)
        message("Doxygen not installed. Project documentation not generated")
    endif (DOXYGEN_FOUND)
endif ()


################################################################################
## Build #######################################################################
################################################################################

add_subdirectory(src)

################################################################################
## Status ######################################################################
################################################################################

message(STATUS)
message(STATUS "${PROJECT_NAME} Configuration:")
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "BUILD_TESTS: ${BUILD_TESTS}")
message(STATUS "BUILD_DOCUMENTATION: ${BUILD_DOCUMENTATION}")
message(STATUS)
