project(Modified-ALT)

add_library(graph
    graph.cpp
    graph_parser.cpp
    graph.h
    graph_parser.h
)

target_include_directories(graph PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
