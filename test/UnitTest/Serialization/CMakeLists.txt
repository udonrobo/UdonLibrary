cmake_minimum_required(VERSION 3.14)

# set test source files
add_executable(
  Test
  Serialization.cpp
)

# Add include directory
target_include_directories(Test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../../src)
# set(UDON_LIBRARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../src PARENT_SCOPE)
message("UDON_LIBRARY_DIR: ${UDON_LIBRARY_DIR}")

# link with gtest
target_link_libraries(
  Test
  gtest_main
)

# discover tests
gtest_discover_tests(Test)
