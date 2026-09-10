cmake_minimum_required(VERSION 4.3)

project(Yoka)

set(CMAKE_CXX_STANDARD 20)

add_executable(Yoka
        main.cpp
        Core/Window.cpp
        Core/Window.h
        Core/RenderEngine.cpp
        Core/RenderEngine.h
        Include/stb_truetype.h)

target_link_libraries(Yoka PRIVATE
        opengl32
        gdi32
        "${CMAKE_SOURCE_DIR}/libglfw3.a"
)