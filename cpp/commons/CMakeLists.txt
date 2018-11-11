cmake_minimum_required(VERSION 3.9)
find_package(GTest REQUIRED)

project(commons CXX)

add_library(commons SHARED array-allocator.hpp random.hpp simd-vector.hpp)
set_target_properties(commons PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(commons PUBLIC ${PROJECT_SOURCE_DIR})

add_executable(commons-test commons-test.cpp)
target_link_libraries(commons-test GTest::Main commons)
target_compile_options(commons-test PUBLIC "-march=native")
