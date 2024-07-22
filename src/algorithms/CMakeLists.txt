project(Modified-ALT)

add_library(algorithms
    dijkstra.cpp
    ALT.cpp
    new_ALT.cpp
    dijkstra.h
    ALT.h
    new_ALT.h
)

target_include_directories(algorithms PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(algorithms PUBLIC graph preprocessing)
