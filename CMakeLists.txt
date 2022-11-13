cmake_minimum_required(VERSION 3.20)

project(
  ccc
  DESCRIPTION "The ANSI-C Cursed Compiler"
  LANGUAGES CXX
)

project(
  ccc-cmd
  DESCRIPTION "CCC command line interface"
  LANGUAGES CXX
)

project(
  ccc-test
  DESCRIPTION "CCC test suite"
  LANGUAGES CXX
)

include(cmake/project.cmake)
include(cmake/dependencies.cmake)

add_subdirectory(src/ccc/)
add_subdirectory(src/cmd/)
add_subdirectory(src/test/)
