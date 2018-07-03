project(simple_network)

set(EXECUTABLE simple_network.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

add_custom_target(run_simple_network
    COMMAND simple_network
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_executable(simple_network ${EXECUTABLE})
target_link_libraries(simple_network real_engine)