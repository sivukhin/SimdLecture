set(CXX_STANDARD 17)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND APPLE)
  message(FATAL_ERROR "Use gcc from brew")
endif()

if (CMAKE_COMPILER_IS_GNUCC AND (NOT (CMAKE_CXX_COMPILER_ID MATCHES "Clang")) AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 7.1)
  message(FATAL_ERROR "Install gcc-7.2 or newer")
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.0)
  message(FATAL_ERROR "Install clang-5.0 or newer")
endif()

if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++${CXX_STANDARD} -Wall -Wextra -Wpedantic -g")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++${CXX_STANDARD} -Wall -g")
endif()


set(CMAKE_CXX_FLAGS_ASAN "-g -fsanitize=address,undefined -fno-sanitize-recover=all"
    CACHE STRING "Compiler flags in asan build"
    FORCE)

set(CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_ASAN} -fprofile-instr-generate -fcoverage-mapping")
