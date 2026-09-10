//
// Created by dlllibstdntc on 10.09.2026.
//

#include "RenderSystem.h"

#include <cmath>

#include "Doomsday.h"

void BufferBuilder::FirstInitialize() {
    if (firstCall == FALSE) {
        if (coordinate_system == CoordinateSystem::WORLD) {
            glPushMatrix();

            glMatrixMode(GL_PROJECTION);
            glPushMatrix();
            glLoadIdentity();

            const FLOAT width =
                Doomsday::GetInstance().GetWindow()->GetWidth();

            const FLOAT height =
                Doomsday::GetInstance().GetWindow()->GetHeight();

            glOrtho(
                0,
                width,
                height,
                0,
                -1,
                1
            );

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
        }
        glBegin(mode);
        firstCall = TRUE;
    }
}

FLOAT BufferBuilder::Tx(FLOAT x) {
    return x;
}

FLOAT BufferBuilder::Ty(FLOAT y) {
    return y;
}

BufferBuilder & BufferBuilder::Color3(FLOAT r, FLOAT g, FLOAT b) {
    FirstInitialize();
    glColor3f(r, g, b);

    return *this;
}

BufferBuilder & BufferBuilder::Color4(FLOAT r, FLOAT g, FLOAT b, FLOAT a) {
    FirstInitialize();
    glColor4f(r, g, b, a);

    return *this;
}

BufferBuilder & BufferBuilder::Vertex2(FLOAT x, FLOAT y) {
    FirstInitialize();
    glVertex2f(Tx(x), Ty(y));

    return *this;
}

BufferBuilder & BufferBuilder::Vertex3(FLOAT x, FLOAT y, FLOAT z) {
    FirstInitialize();
    glVertex3f(Tx(x), Ty(y), z);

    return *this;
}

BufferBuilder & BufferBuilder::Rect2x2(FLOAT x, FLOAT y, FLOAT width, FLOAT height) {
    FirstInitialize();

    Vertex2(x - width, y - height);
    Vertex2(x + width, y - height);
    Vertex2(x + width, y + height);
    Vertex2(x - width, y + height);

    return *this;
}

BufferBuilder & BufferBuilder::Line2(FLOAT minX, FLOAT maxX, FLOAT width)
{
    FirstInitialize();

    glLineWidth(width);
    glVertex2f(minX, maxX);

    return *this;
}

BufferBuilder BufferBuilder::Circle2x2(FLOAT x, FLOAT y, FLOAT rad) {
    constexpr FLOAT attempt = 360;

    for (unsigned i = 0; i < attempt; i++) {
        FLOAT angle = 2 * 3.14 * i / attempt;
        FLOAT px = x + cosf(angle) * rad;
        FLOAT py = y + sinf(angle) * rad;

        glVertex2f(px, py);
    }

    return *this;
}

BufferBuilder & BufferBuilder::End() {
    if (coordinate_system == CoordinateSystem::WORLD) {
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();

        glMatrixMode(GL_PROJECTION);
        glPopMatrix();

        glMatrixMode(GL_MODELVIEW);
    }
    glEnd();
    return *this;
}

void RenderSystem::Setup(Window *window) {
    camera = std::make_unique<Camera>();
    camera->x = 0;
    camera->y = 0;
    camera->height = 100;
    camera->zoom = 1;

    Resize(window,
        static_cast<INT>(window->GetWidth()),
        static_cast<INT>(window->GetHeight()));
}

void RenderSystem::Resize(Window *window, INT w, INT h) const {
    glViewport(0, 0, w, h);
    UpdateProjective(w, h);
}

void RenderSystem::Update(Window *window) {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor4f(0, 0, 0, 1);

    auto now = glfwGetTime();
    deltaTime = now - lastTime;
    lastTime = now;

    glLoadIdentity();
    glTranslatef(-camera->x, -camera->y, 0);
}

TICK RenderSystem::GetDeltaTime() const {
    return deltaTime;
}

void RenderSystem::UpdateProjective(INT w, INT h) const {
    const auto aspect = static_cast<float>(w) / h;
    const auto visibleHeight = camera->height / camera->zoom;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    const auto halfHeight = visibleHeight / 2;
    const auto halfWidth = halfHeight * aspect;

    glOrtho(-halfWidth, halfWidth, -halfHeight, halfHeight, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

Camera * RenderSystem::GetCamera() const {
    return camera.get();
}

FLOAT RenderSystem::ToWorldX(FLOAT x) const {
    const FLOAT width =
        Doomsday::GetInstance().GetWindow()->GetWidth();

    const FLOAT height =
        Doomsday::GetInstance().GetWindow()->GetHeight();

    const FLOAT aspect = width / height;

    const FLOAT visibleHeight =
        camera->height / camera->zoom;

    const FLOAT visibleWidth =
        visibleHeight * aspect;

    return ((x / width) - 0.5f) * visibleWidth + camera->x;
}

FLOAT RenderSystem::ToWorldY(FLOAT y) const {
    const FLOAT width =
        Doomsday::GetInstance().GetWindow()->GetWidth();

    const FLOAT height =
        Doomsday::GetInstance().GetWindow()->GetHeight();

    const FLOAT aspect = width / height;

    const FLOAT visibleHeight =
        camera->height / camera->zoom;

    return (0.5f - y / height) * visibleHeight + camera->y;
}

BufferBuilder RenderSystem::Buffer(INT mode, CoordinateSystem coordinate_system) {
    BufferBuilder bb(this, mode, coordinate_system);
    return bb;
}

