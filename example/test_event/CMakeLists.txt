project(test_event)

set(EXECUTABLE test_event.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

add_custom_target(run_test_event
    COMMAND test_event
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_executable(test_event ${EXECUTABLE})
target_link_libraries(test_event real_engine)