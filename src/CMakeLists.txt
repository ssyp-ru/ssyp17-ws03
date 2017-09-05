cmake_minimum_required(VERSION 2.8)

project(real_engine)

set(SOURCE_LIB  window/opengl.cpp
                graphics/image.cpp
                graphics/graphic.cpp
                graphics/animation.cpp
                utils/logger.cpp
                math/math.cpp
                utils/stop_watch.cpp
                recource_manager/xml_parser.cpp
                recource_manager/tiled_parser.cpp
                recource_manager/config_manager.cpp
                recource_manager/resource_manager.cpp
                graphics/particle.cpp
                mainApp.cpp
                physic/physic_core.cpp
                gui/base_button.cpp
                gui/gui_manager.cpp
)

set_source_files_properties(graphics/Image.cpp PROPERTIES COMPILE_FLAGS -fno-strict-aliasing)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})

add_library(real_engine SHARED ${SOURCE_LIB})
# For stb_image lib
target_link_libraries(real_engine gcc)
target_link_libraries(real_engine gcc_s)


target_link_libraries(real_engine ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})

#target_link_libraries(real_engine stdc++fs)