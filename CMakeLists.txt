cmake_minimum_required(VERSION 3.10)

project(Modified-ALT VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(Modified-ALT src/main.cpp)

target_include_directories(Modified-ALT PRIVATE include)

target_compile_definitions(Modified-ALT PRIVATE RESOURCE_PATH="${CMAKE_SOURCE_DIR}/resources")
