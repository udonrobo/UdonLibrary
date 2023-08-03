cmake_minimum_required(VERSION 3.14)

add_executable(
  Test
  Serialization.cpp
)

target_link_libraries(
  Test
  gtest_main
)

gtest_discover_tests(Test)
