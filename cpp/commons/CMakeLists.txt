cmake_minimum_required(VERSION 3.9)

project(commons CXX)

add_library(commons SHARED array-allocator.hpp random.hpp)
set_target_properties(commons PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(commons PUBLIC ${PROJECT_SOURCE_DIR})