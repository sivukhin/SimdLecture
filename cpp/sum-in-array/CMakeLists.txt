find_package(benchmark)

add_executable(sum-in-array-bench sum-in-array-bench.cpp)
target_link_libraries(sum-in-array-bench benchmark pthread)
target_compile_options(sum-in-array-bench PUBLIC "-march=native")
