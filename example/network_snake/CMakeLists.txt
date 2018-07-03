project(network_snake)

set(EXECUTABLE main.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

add_custom_target(run_network_snake
    COMMAND network_snake
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_executable(network_snake ${EXECUTABLE})
target_link_libraries(network_snake real_engine)