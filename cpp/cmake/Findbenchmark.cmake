cmake_minimum_required(VERSION 3.9)
find_package(PkgConfig QUIET)
if (PkgConfig_FOUND)
    pkg_search_module(BENCHMARK QUIET benchmark)
endif()

if (NOT BENCHMARK_FOUND)
    find_path(BENCHMARK_INCLUDE_DIRS NAMES benchmark/)
    mark_as_advanced(BENCHMARK_INCLUDE_DIRS)

    find_library(BENCHMARK_LIBRARIES NAMES benchmark)
    mark_as_advanced(BENCHMARK_LIBRARIES)
endif()

FIND_PACKAGE_HANDLE_STANDARD_ARGS(BENCHMARK REQUIRED_VARS
        BENCHMARK_LIBRARIES BENCHMARK_INCLUDE_DIRS)
