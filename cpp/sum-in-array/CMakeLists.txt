find_package(benchmark REQUIRED)
find_package(GTest REQUIRED)


add_executable(sum-in-array-test sum-in-array-test.cpp)
target_link_libraries(sum-in-array-test GTest::Main commons)
target_compile_options(sum-in-array-test PUBLIC "-march=native")


add_executable(sum-in-array-bench sum-in-array-bench.cpp)
target_link_libraries(sum-in-array-bench benchmark pthread commons)
target_compile_options(sum-in-array-bench PUBLIC "-march=native" "-O3")