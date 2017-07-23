cmake_minimum_required(VERSION 2.8)


project(main)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake_files")

# для компиляции своего примера нужно будет в эту строчку поставить путь к своему файлу примера 
# (в дальнейшем система сборки будет немного изменена)
set(EXECUTABLE example/simple_window/simple_window.cpp)

set(MATH_TEST_EXECUTABLE tests/math_test.cpp)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out)

add_custom_target(run
    COMMAND main
    DEPENDS main
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)


add_compile_options(-std=c++14)
add_compile_options(-g)
# add_compile_options(-Wall)

include_directories(include)
include_directories(ext)

add_subdirectory(src)

add_executable(main ${EXECUTABLE})
target_link_libraries(main real_engine)

add_executable(math_test ${MATH_TEST_EXECUTABLE})
target_link_libraries(math_test real_engine)