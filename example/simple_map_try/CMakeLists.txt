project(simple_map_try)

set(EXECUTABLE  simple_map.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

add_custom_target(run_simple_map_try
    COMMAND simple_map_try
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_executable(simple_map_try ${EXECUTABLE})
target_link_libraries(simple_map_try real_engine)
