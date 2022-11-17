file(
  GLOB_RECURSE CCC_TEST_SOURCE
  ${CCC_REGEX_SOURCE}*.hpp
  ${CCC_REGEX_SOURCE}*.cpp
)

add_executable(
  ccc-test
  ${CCC_TEST_SOURCE}
)

target_include_directories(
  ccc-test PRIVATE
  ${CCC_PROJECT_DIR}/src/ccc/
)

target_link_libraries(
  ccc-test PRIVATE
  ccc
  gtest
)

set_target_properties(
  ccc-test PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED YES
  LINKER_LANGUAGE CXX
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
