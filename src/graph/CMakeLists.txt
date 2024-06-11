project(Modified-ALT)

add_library(graph
    edge.cpp
    edge.h
    graph_parser.h
    graph_parser.cpp
)

target_include_directories(graph PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
