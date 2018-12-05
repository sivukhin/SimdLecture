find_package(benchmark REQUIRED)
find_package(GTest REQUIRED)

add_executable(words-count-test words-count-test.cpp)
target_link_libraries(words-count-test GTest::Main commons)
target_compile_options(words-count-test PUBLIC "-march=native")

add_executable(words-count-bench words-count-bench.cpp)
target_link_libraries(words-count-bench benchmark pthread commons)
target_compile_options(words-count-bench PUBLIC "-march=native" "-O3")
