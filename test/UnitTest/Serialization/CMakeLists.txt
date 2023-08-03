#-------------------------------------------------------------------
#
#    UdonLibrary
#
#    Copyright (c) 2022-2023 udonrobo
#
#    Licensed under the MIT License.
#
#-------------------------------------------------------------------
#
#    Serialization Test
#
#-------------------------------------------------------------------

cmake_minimum_required(VERSION 3.14)

# set test source files
add_executable(Test
    Serialization.cpp
)

# Add include directory
target_include_directories(Test PUBLIC ${UDON_LIBRARY_DIR})

# link with gtest
target_link_libraries(Test gtest_main)

# discover tests
gtest_discover_tests(Test)
