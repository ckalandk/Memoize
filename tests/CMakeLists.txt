cmake_minimum_required(VERSION 3.22.1)

project(tests)

find_package(Catch2 CONFIG REQUIRED)

add_executable(lrucache-test lru.test.cpp)
add_executable(memoize-test memoize.test.cpp)

target_include_directories(
lrucache-test 
  INTERFACE 
    ${CMAKE_SOURCE_DIR}/src
)

target_include_directories(
memoize-test
  INTERFACE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(
lrucache-test 
  INTERFACE Memoize::Memoize 
  PRIVATE Catch2::Catch2 Catch2::Catch2WithMain
)

target_link_libraries(
memoize-test
  INTERFACE Memoize::Memoize
  PRIVATE Catch2::Catch2 Catch2::Catch2WithMain
)

include(CTest)
enable_testing()
include(Catch)
catch_discover_tests(lrucache-test)
catch_discover_tests(memoize-test)