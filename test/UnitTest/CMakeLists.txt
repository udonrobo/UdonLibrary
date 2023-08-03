cmake_minimum_required(VERSION 3.14)
project(GoogleUnitTest)

# GoogleTest settings
set(GoogleTestDir ${CMAKE_CURRENT_SOURCE_DIR}/Thirdparty/googletest)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(${GoogleTestDir})
include(GoogleTest)
enable_testing()

# Add include directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)

# Add test directory
add_subdirectory(./UnitTest)
