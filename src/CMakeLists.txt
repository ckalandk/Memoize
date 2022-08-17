cmake_minimum_required(VERSION 3.22.1)

project(Memoize 
	VERSION 1.0.0
	DESCRIPTION "memoize a function")


add_library(memoize INTERFACE)
target_include_directories(memoize INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_features(memoize INTERFACE cxx_std_20)

add_library(Memoize::Memoize ALIAS memoize)
