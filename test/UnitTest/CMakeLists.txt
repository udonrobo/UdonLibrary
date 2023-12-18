#
#    UnitTest Pearent CMakeLists.txt
#
#    Copyright (c) 2022-2023 udonrobo
#

cmake_minimum_required(VERSION 3.14)
project(UnitTest)

if (MSVC)
    add_compile_options(/std:c++14)   # Enable C++14
    add_compile_options(/permissive-) # Enable conformance mode
endif()


# GoogleTest settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Thirdparty/googletest)
include(GoogleTest)
enable_testing()

# Set UdonLibrary directory path to cache -> ${UDON_LIBRARY_DIR} 
get_filename_component(ABSOLUTE_UDON_LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../src" ABSOLUTE)
set(UDON_LIBRARY_DIR ${ABSOLUTE_UDON_LIBRARY_DIR} CACHE STRING "UdonLibrary include path")

# Add test directory
add_subdirectory(./Serialization)
add_subdirectory(./Types)
add_subdirectory(./Common)
