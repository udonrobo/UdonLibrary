#
#    Types Test
#
#    Copyright (c) 2022-2024 udonrobo
#

cmake_minimum_required(VERSION 3.14)

# set test source files
add_executable(TypesTest
    Vector2D.cpp
    StringView.cpp
    Optional.cpp
)

# Add include directory
target_include_directories(TypesTest PUBLIC ${UDON_LIBRARY_DIR})

# link with gtest
target_link_libraries(TypesTest gtest_main)

# discover tests
gtest_discover_tests(TypesTest)
