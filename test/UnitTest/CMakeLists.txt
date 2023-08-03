####################################################################
#
#    UdonLibrary
#
#    Copyright (c) 2022-2023 udonrobo
#
#    Licensed under the MIT License.
#
####################################################################
#
#    UnitTest
#
####################################################################

cmake_minimum_required(VERSION 3.14)

project(UnitTest)

# GoogleTest settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Thirdparty/googletest)
include(GoogleTest)
enable_testing()

# Add include directory
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)

# Add test directory
add_subdirectory(./Serialization)
