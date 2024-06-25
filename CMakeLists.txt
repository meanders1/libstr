# Used this: https://gitlab.com/CLIUtils/modern-cmake/-/blob/master/examples/simple-project/CMakeLists.txt
# as a template

cmake_minimum_required(VERSION 3.1...3.29)

project(
  libstr
  VERSION 1.0
  LANGUAGES CXX)

add_library(libstrlibrary "libstr.hpp" "empty.cpp")
set_target_properties(libstrlibrary PROPERTIES LINKER_LANGUAGE CXX)

add_executable(libstrExample "example.cpp" "empty.cpp")

target_link_libraries(libstrExample PRIVATE libstrlibrary)

enable_testing()
add_test(NAME libstrExample COMMAND libstrExample)
