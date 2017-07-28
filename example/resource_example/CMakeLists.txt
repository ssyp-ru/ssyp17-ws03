project(resource_example)

set(EXECUTABLE resource_example.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

add_custom_target(run_resource_example
    COMMAND resource_example
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_executable(resource_example ${EXECUTABLE})
target_link_libraries(resource_example real_engine)