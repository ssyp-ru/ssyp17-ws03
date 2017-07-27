project(physic_core_example)

set(EXECUTABLE  core.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

add_custom_target(run_physic_core_example
    COMMAND physic_core_example
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_executable(physic_core_example ${EXECUTABLE})
target_link_libraries(physic_core_example real_engine)
