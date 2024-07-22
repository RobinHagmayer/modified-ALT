project(Modified-ALT)

add_library(benchmarking
    benchmark.cpp
    benchmark_file_generator.cpp
    benchmark.h
    benchmark_file_generator.h
)

target_include_directories(benchmarking PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(benchmarking PUBLIC graph algorithms preprocessing argparse)
