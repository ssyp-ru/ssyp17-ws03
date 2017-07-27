cmake_minimum_required(VERSION 2.8)

project(real_engine)

set(SOURCE_LIB  window/opengl.cpp
                graphics/image.cpp
                graphics/graphic.cpp
                utils/logger.cpp
                math/math.cpp
                utils/stop_watch.cpp
                recource_manager/xml_parser.cpp
                recource_manager/tiled_parser.cpp
                mainApp.cpp
)

set_source_files_properties(graphics/Image.cpp PROPERTIES COMPILE_FLAGS -fno-strict-aliasing)

add_library(real_engine SHARED ${SOURCE_LIB})

# For stb_image lib
target_link_libraries(real_engine gcc)
target_link_libraries(real_engine gcc_s)

target_link_libraries(real_engine glut)
target_link_libraries(real_engine GLU)
target_link_libraries(real_engine GL)

target_link_libraries(real_engine stdc++fs)
target_link_libraries(real_engine xcb-image)