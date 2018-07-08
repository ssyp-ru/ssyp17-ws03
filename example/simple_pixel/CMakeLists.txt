project(simple_pixel)

set(EXECUTABLE main.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

add_custom_target(run_simple_pixel
    COMMAND simple_pixel
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_executable(simple_pixel ${EXECUTABLE})
target_link_libraries(simple_pixel real_engine)