project(main_menu)

set(EXECUTABLE main_menu.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

add_custom_target(run_main_menu
    COMMAND main_menu
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_executable(main_menu ${EXECUTABLE})
target_link_libraries(main_menu real_engine)