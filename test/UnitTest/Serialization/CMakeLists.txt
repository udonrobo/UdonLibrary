cmake_minimum_required(VERSION 3.14)

add_executable(
  Test
  Serialization.cpp
)

target_link_libraries(
  Test
  gtest_main
)

# Add include directory
target_include_directories(Test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../../src)

gtest_discover_tests(Test)
