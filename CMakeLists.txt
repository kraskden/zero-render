cmake_minimum_required(VERSION 3.17)
project(glpainter)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Gui REQUIRED)



add_executable(glpainter src/main.cpp src/gui/MainWindow.cpp src/gui/MainWindow.h src/obj/ObjModel.cpp src/obj/ObjModel.h src/obj/Parser.cpp src/obj/Parser.h src/gl/GlPainter.cpp src/gl/GlPainter.h src/gl/Scene.cpp src/gl/Scene.h src/math/Matrix4D.cpp src/math/Matrix4D.h src/gl/Camera.cpp src/gl/Camera.h)

target_link_libraries(glpainter Qt5::Gui)