cmake_minimum_required(VERSION 3.22.1)

project(tests)


add_executable(memoize_test main.cpp )

find_package(benchmark REQUIRED)

target_include_directories(
    memoize_test INTERFACE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(
memoize_test INTERFACE
  Memoize::Memoize
)

target_compile_features(memoize_test PRIVATE cxx_std_20)
target_link_libraries(memoize_test PRIVATE benchmark::benchmark)
