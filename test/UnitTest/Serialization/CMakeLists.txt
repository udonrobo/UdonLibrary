#
#    Serialization Test
#
#    Copyright (c) 2022 udonrobo
#

cmake_minimum_required(VERSION 3.14)

# set test source files
add_executable(SerializationTest
    Serialization.cpp
)

# Add include directory
target_include_directories(SerializationTest PUBLIC ${UDON_LIBRARY_DIR})

# link with gtest
target_link_libraries(SerializationTest gtest_main)

# discover tests
gtest_discover_tests(SerializationTest)
