project(simple_map)

set(EXECUTABLE  simple_map.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

add_custom_target(run_simple_map
    COMMAND simple_map
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_executable(simple_map ${EXECUTABLE})
target_link_libraries(simple_map real_engine)
