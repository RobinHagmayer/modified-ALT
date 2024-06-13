project(Modified-ALT)

add_library(cli
    cli.cpp
    cli.h
)

target_include_directories(cli PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(cli PUBLIC graph algorithms preprocessing)
