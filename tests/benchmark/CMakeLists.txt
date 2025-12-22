find_package(benchmark REQUIRED)

add_executable(bench_tests main.cpp)
target_link_libraries(bench_tests PRIVATE benchmark::benchmark_main foundation quant_core)
