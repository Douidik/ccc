file(
  GLOB_RECURSE CCC_CMD_SOURCE
  ${CCC_REGEX_SOURCE}*.hpp
  ${CCC_REGEX_SOURCE}*.cpp
)

add_executable(
  ccc-cmd
  ${CCC_CMD_SOURCE}
)

target_include_directories(
  ccc-cmd PRIVATE
  ${CCC_PROJECT_DIR}/src/ccc/
)

target_link_libraries(
  ccc-cmd PRIVATE
  ccc
)

set_target_properties(
  ccc-cmd PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED YES
  LINKER_LANGUAGE CXX
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
