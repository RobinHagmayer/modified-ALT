cmake_minimum_required(VERSION 3.14)
project(modified-ALT)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable export of compile commands for clangd LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set build type if not specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# Set optimization and debug flags
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra -Wshadow -Wconversion -Wpedantic -Werror")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -Wall -Wextra -Wshadow -Wconversion -Wpedantic -Werror")
# set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -pg -Wall -Wextra -Wshadow -Wconversion -Wpedantic -Werror")

# Fetch Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        b514bdc898e2951020cbdca1304b75f5950d1f59
)
FetchContent_MakeAvailable(googletest)

# Fetch argparse
include(FetchContent)
FetchContent_Declare(
    argparse
    GIT_REPOSITORY https://github.com/p-ranav/argparse.git
    GIT_TAG        68fd0277eea5412aff9b91c0b70efc698359dae0
)
FetchContent_MakeAvailable(argparse)

# Add src and tests directories
add_subdirectory(src)
add_subdirectory(tests)

# Include the header directories
# include_directories(${PROJECT_SOURCE_DIR}/src/include)
# include_directories(${PROJECT_SOURCE_DIR}/external/include)

