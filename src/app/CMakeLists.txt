project(Modified-ALT)

add_library(app
    app.cpp
    app.h
)

target_include_directories(app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(app PUBLIC cli graph)

target_compile_definitions(app PRIVATE GRAPH_DIR="${CMAKE_SOURCE_DIR}/data/graphs/")
