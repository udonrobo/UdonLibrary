#
#    CommonTest Test
#
#    Copyright (c) 2022-2024 udonrobo
#

cmake_minimum_required(VERSION 3.14)

# set test source files
add_executable(CommonTest
    Include.cpp
)

# Add include directory
target_include_directories(CommonTest PUBLIC ${UDON_LIBRARY_DIR})

# link with gtest
target_link_libraries(CommonTest gtest_main)

# discover tests
gtest_discover_tests(CommonTest)
