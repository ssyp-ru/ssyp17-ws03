cmake_minimum_required(VERSION 2.8)

project(main)

if ( CMAKE_COMPILER_IS_GNUCC )
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wno-switch") 
endif ( CMAKE_COMPILER_IS_GNUCC )


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out)


add_compile_options(-std=c++14)
add_compile_options(-g)
add_compile_options(-Wall)
add_compile_options(-Wpedantic)

include_directories(include)
include_directories(ext)


# Real Engine library
add_subdirectory(src)


# tests
set(MATH_TEST_EXECUTABLE tests/math_test.cpp)

add_executable(math_test ${MATH_TEST_EXECUTABLE})
target_link_libraries(math_test real_engine)


# examples
add_subdirectory(example/simple_window)
add_subdirectory(example/simple_network)
add_subdirectory(example/simple_platformer)
add_subdirectory(example/parser_example)
add_subdirectory(example/resource_example)
add_subdirectory(example/network_snake)
add_subdirectory(example/physic_core)
add_subdirectory(example/game_prototype)
add_subdirectory(example/simple_map_try)
add_subdirectory(example/test_event)