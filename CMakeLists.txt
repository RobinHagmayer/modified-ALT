cmake_minimum_required(VERSION 3.10)

project(Modified-ALT VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-O3")
# set(CMAKE_CXX_FLAGS "-O0 -g")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
# file(GLOB_RECURSE SOURCES ${SOURCE_DIR}/*.cpp)

# add_executable(modified-alt ${SOURCES})
add_executable(modified-alt src/main.cpp src/GraphParser.cpp src/Edge.cpp src/cli.cpp src/Dijkstra.cpp)

target_include_directories(modified-alt PRIVATE include)
target_compile_definitions(modified-alt PRIVATE RESOURCE_PATH="${CMAKE_SOURCE_DIR}/resources")

add_executable(modified-alt-bench src/benchmark.cpp src/GraphParser.cpp src/Edge.cpp src/Dijkstra.cpp)

target_include_directories(modified-alt-bench PRIVATE include)
target_compile_definitions(modified-alt-bench PRIVATE RESOURCE_PATH="${CMAKE_SOURCE_DIR}/resources")
