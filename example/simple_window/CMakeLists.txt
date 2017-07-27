project(simple_window)

set(EXECUTABLE simple_window.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

add_custom_target(run_simple_window
    COMMAND simple_window
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_executable(simple_window ${EXECUTABLE})
target_link_libraries(simple_window real_engine)