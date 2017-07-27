project(parser_example)

set(EXECUTABLE parser_example.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

add_custom_target(run_parser_example
    COMMAND parser_example
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_executable(parser_example ${EXECUTABLE})
target_link_libraries(parser_example real_engine)