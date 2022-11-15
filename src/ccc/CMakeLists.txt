file(
  GLOB_RECURSE CCC_SOURCE
  ${CCC_SOURCE_REGEX}*.hpp
  ${CCC_SOURCE_REGEX}*.cpp
)

add_library(
  ccc STATIC
  ${CCC_SOURCE}
)

target_include_directories(
  ccc PRIVATE
  ${CCC_PROJECT_DIR}/src/ccc/
)

target_link_libraries(
  ccc PUBLIC
  fmt::fmt
  magic_enum::magic_enum
)

set_target_properties(
  ccc PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED YES
  LINKER_LANGUAGE CXX
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)
