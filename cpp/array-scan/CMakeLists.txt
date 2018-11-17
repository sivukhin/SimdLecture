find_package(benchmark REQUIRED)

add_executable(array-scan-bench array-scan-bench.cpp)
target_link_libraries(array-scan-bench benchmark pthread commons)
target_compile_options(array-scan-bench PUBLIC "-march=native" "-O3")