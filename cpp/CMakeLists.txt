cmake_minimum_required(VERSION 3.9)
include(cmake/BuildFlags.cmake)

add_subdirectory(commons)
add_subdirectory(cpu-capabilities)
add_subdirectory(sum-in-array)
add_subdirectory(array-scan)
add_subdirectory(words-count)