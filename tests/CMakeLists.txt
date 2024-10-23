enable_testing()

set(TEST_SOURCES
    test_graph.cc
)

set(SOURCE_SOURCES
    ${PROJECT_SOURCE_DIR}/src/graph/graph.cc
    ${PROJECT_SOURCE_DIR}/src/graph/graph_parser.cc
    ${PROJECT_SOURCE_DIR}/src/algorithms/dijkstra.cc
)

foreach(TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_SOURCE} ${SOURCE_SOURCES})
    target_link_libraries(${TEST_NAME} GTest::gtest_main)

    target_include_directories(${TEST_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src/include)

    include(GoogleTest)
    gtest_discover_tests(${TEST_NAME})
endforeach()
