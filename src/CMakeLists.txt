project(Modified-ALT)

add_library(main main.cpp)

target_include_directories(main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(main PUBLIC app)

add_subdirectory(app)
add_subdirectory(cli)
add_subdirectory(graph)
add_subdirectory(algorithms)
add_subdirectory(preprocessing)
add_subdirectory(benchmarking)
