add_executable(modified-ALT
    algorithms/dijkstra.cc
    algorithms/alt.cc
    algorithms/modified-alt.cc
    benchmark/benchmark.cc
    benchmark/bench_utils.cc
    graph/graph.cc
    graph/graph_parser.cc
    graph/graph_partitioner.cc
    preprocessing/distance_calculation.cc
    preprocessing/farthest.cc
    preprocessing/landmark_selection.cc
    preprocessing/ldv_serializer.cc
    preprocessing/modified_alt_data_serializer.cc
    preprocessing/utils.cc
    main.cc
)

# Set the output directory for the executable
set_target_properties(modified-ALT PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

# Include the header files
target_include_directories(modified-ALT PRIVATE ${PROJECT_SOURCE_DIR}/src/include)

# Link argparse
target_link_libraries(modified-ALT argparse)

# Link OpenMP
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(modified-ALT OpenMP::OpenMP_CXX)
endif()
