//
// Created by dlllibstdntc on 10.09.2026.
//

#ifndef GAMEENGINE_RENDERSYSTEM_H
#define GAMEENGINE_RENDERSYSTEM_H

#include <memory>
#include <Windows.h>
#include <GL/gl.h>

class Window;
class RenderSystem;
class Camera;

typedef DOUBLE TICK;

class Camera {
public:
    FLOAT x, y, zoom, height;
};

enum class CoordinateSystem {
    WORLD, SCREEN
};

class BufferBuilder {
    RenderSystem* render_system = nullptr;
    INT mode = 0;
    CoordinateSystem coordinate_system;
    BOOL firstCall = FALSE;
    void FirstInitialize();

    FLOAT Tx(FLOAT x);
    FLOAT Ty(FLOAT y);
public:
    BufferBuilder(RenderSystem* render_system, INT mode, CoordinateSystem coordinate_system)
        : render_system(render_system), mode(mode),coordinate_system(coordinate_system) {}

    BufferBuilder& Color3(FLOAT r, FLOAT g, FLOAT b);
    BufferBuilder& Color4(FLOAT r, FLOAT g, FLOAT b, FLOAT a);
    BufferBuilder& Vertex2(FLOAT x, FLOAT y);
    BufferBuilder& Vertex3(FLOAT x, FLOAT y, FLOAT z);
    BufferBuilder& Rect2x2(FLOAT x, FLOAT y, FLOAT width, FLOAT height);
    BufferBuilder& Line2(FLOAT minX, FLOAT maxX, FLOAT width);
    BufferBuilder Circle2x2(FLOAT x, FLOAT y, FLOAT rad);
    BufferBuilder& End();
};

class RenderSystem {
    TICK lastTime = 0;
    TICK deltaTime = 0;
    std::unique_ptr<Camera> camera;
public:
    void Setup(Window* window);
    void Resize(Window* window, INT w, INT h) const;
    void Update(Window* window);
    TICK GetDeltaTime() const;
    void UpdateProjective(INT w, INT h) const;
    Camera* GetCamera() const;

    FLOAT ToWorldX(FLOAT x) const;
    FLOAT ToWorldY(FLOAT y) const;

    BufferBuilder Buffer(INT mode, CoordinateSystem
        coordinate_system = CoordinateSystem::SCREEN);
};


#endif //GAMEENGINE_RENDERSYSTEM_H
