cmake_minimum_required(VERSION 3.10)

project(Modified-ALT VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# set(CMAKE_CXX_FLAGS "-O3")
set(CMAKE_CXX_FLAGS "-O0 -g")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(Modified-ALT src/main.cpp src/Edge.cpp src/GraphParser.cpp src/Dijkstra.cpp)

target_include_directories(Modified-ALT PRIVATE include)

target_compile_definitions(Modified-ALT PRIVATE RESOURCE_PATH="${CMAKE_SOURCE_DIR}/resources")
