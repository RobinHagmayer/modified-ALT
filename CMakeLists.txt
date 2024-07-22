cmake_minimum_required(VERSION 3.14)

project(Modified-ALT VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-O3")
# set(CMAKE_CXX_FLAGS "-O0 -g")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src)

add_executable(modified-ALT src/main.cpp)
target_link_libraries(modified-ALT PRIVATE main)

add_executable(modified-ALT-bench src/benchmarking/benchmark_runner.cpp)
target_link_libraries(modified-ALT-bench PRIVATE benchmarking)
# target_compile_definitions(modified-ALT-bench PRIVATE BENCH_DIR="${CMAKE_SOURCE_DIR}/data/benchs/")
