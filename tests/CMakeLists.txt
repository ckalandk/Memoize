cmake_minimum_required(VERSION 3.22.2)

project(tests)


add_executable(memoize_test main.cpp )

target_include_directories(
memoize_test
  INTERFACE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(
memoize_test
  Memoize::Memoize
)
