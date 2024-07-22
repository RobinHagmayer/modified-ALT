project(Modified-ALT)

add_library(preprocessing
    random_landmarks.cpp
    farthest.cpp
    new.cpp
    random_landmarks.h
    farthest.h
    new.h
)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(preprocessing PUBLIC OpenMP::OpenMP_CXX)
endif()

target_include_directories(preprocessing PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(preprocessing PUBLIC graph algorithms)
