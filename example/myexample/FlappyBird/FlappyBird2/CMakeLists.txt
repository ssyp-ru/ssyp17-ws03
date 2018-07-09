project(FlappyBird2)

set(EXECUTABLE FlappyBird2.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

add_custom_target(run_FlappyBird2
    COMMAND FlappyBird2
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_executable(FlappyBird2 ${EXECUTABLE})
target_link_libraries(FlappyBird2 real_engine)