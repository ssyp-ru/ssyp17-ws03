project(simple_way)

set(EXECUTABLE simple_way.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

add_custom_target(run_simple_way
    COMMAND simple_way
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_executable(simple_way ${EXECUTABLE})
target_link_libraries(simple_way real_engine)