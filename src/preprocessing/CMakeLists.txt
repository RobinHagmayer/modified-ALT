project(Modified-ALT)

add_library(preprocessing
    random_landmarks.cpp
    farthest.cpp
    new.cpp
    random_landmarks.h
    farthest.h
    new.h
)

target_include_directories(preprocessing PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(preprocessing PUBLIC graph algorithms)
