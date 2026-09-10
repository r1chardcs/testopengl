cmake_minimum_required(VERSION 4.3)
project(GameEngine)

set(CMAKE_CXX_STANDARD 20)

add_executable(GameEngine main.cpp
        Game.cpp
        Game.h
        GameClient/Doomsday.cpp
        GameClient/Doomsday.h
        GameClient/RenderSystem.cpp
        GameClient/RenderSystem.h
        GameClient/World.cpp
        GameClient/World.h)

target_link_libraries(GameEngine PUBLIC opengl32 gdi32 ${CMAKE_SOURCE_DIR}/Lib/libglfw3.lib)

include_directories(Include)