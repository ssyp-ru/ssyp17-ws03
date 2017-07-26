project(simple_platformer)

set(EXECUTABLE  simple_platformer.cpp 
                base_button.cpp 
                player.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

add_custom_target(run_simple_platformer
    COMMAND simple_platformer
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_executable(simple_platformer ${EXECUTABLE})
target_link_libraries(simple_platformer real_engine)