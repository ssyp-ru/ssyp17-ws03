cmake_minimum_required(VERSION 2.8)

project(real_engine)

set(SOURCE_LIB  window/windowXCB.cpp
                window/windowGLUT.cpp
                graphics/image.cpp
                graphics/graphic.cpp
                graphics/animation.cpp
                utils/logger.cpp
                ../example/simple_platformer/base_button.cpp
                math/math.cpp
                utils/stop_watch.cpp
                ../example/simple_platformer/player.cpp
                recource_manager/xml_parser.cpp
                main_app.cpp
)

set_source_files_properties(graphics/Image.cpp PROPERTIES COMPILE_FLAGS -fno-strict-aliasing)

find_package(XCB REQUIRED)
link_libraries(${XCB_LIBRARIES})

find_package(XKB REQUIRED)
link_libraries(${XKB_LIBRARIES})

add_library(real_engine SHARED ${SOURCE_LIB})

# For stb_image lib
target_link_libraries(real_engine gcc)
target_link_libraries(real_engine gcc_s)

target_link_libraries(real_engine glut)
target_link_libraries(real_engine GLU)
target_link_libraries(real_engine GL)

target_link_libraries(real_engine xkbcommon-x11)
target_link_libraries(real_engine stdc++fs)
target_link_libraries(real_engine xcb-image)
