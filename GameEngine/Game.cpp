//
// Created by dlllibstdntc on 10.09.2026.
//

#include "Game.h"

#include <cmath>

FLOAT px = 0, py = 0;
FLOAT width = 800, height = 800, aspect = 0;

void Game::UpdateProjective(INT w, INT h) {
    width = w;
    height = h;

    aspect = static_cast<FLOAT>(width) / height;

    FLOAT visibleHeight = camera.height / camera.zoom;

    FLOAT halfHeight = visibleHeight / 2.0f;
    FLOAT halfWidth = halfHeight * aspect;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(
        -halfWidth,
         halfWidth,
        -halfHeight,
         halfHeight,
        -1.0f,
         1.0f
    );

    glMatrixMode(GL_MODELVIEW);
}

void Game::Run() {
    camera.x = 0;
    camera.y = 0;

    glfwInit();
    handle = glfwCreateWindow(800, 800, "megantcs game", NULL, NULL);
    glfwMakeContextCurrent(handle);
    UpdateProjective(800, 800);
    glfwSetFramebufferSizeCallback(handle, [](GLFWwindow* handle, INT w2, INT h2) {
        width = w2;
        height = h2;
    });

    while (TRUE) {
        if (glfwWindowShouldClose(handle)) break;
        UpdateProjective(width, height);

        DOUBLE currentTime = glfwGetTime();

        deltaTick = currentTime - lastTime;
        lastTime = currentTime;

        glClear(GL_COLOR_BUFFER_BIT);
        Render();

        glfwSwapBuffers(handle);
        glfwPollEvents();
    }
    glfwTerminate();
}

void rectangle(FLOAT x, FLOAT y, FLOAT width, FLOAT height) {
    glBegin(GL_QUADS);
    glColor3f(1, 0, 0);
    glVertex2f(x - width, y - height);
    glVertex2f(x + width, y - height);
    glVertex2f(x + width, y + height);
    glVertex2f(x - width, y + height);
    glEnd();
}

void circle(FLOAT x, FLOAT y, FLOAT rad) {
    const float attempt = 360;
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < attempt; i++) {
        float angle = 2 * 3.14 * i / attempt;

        float px = x + cosf(angle) * rad;
        float py = y + sinf(angle) * rad;

        glVertex2f(px, py);
    }
    glEnd();
}

void Game::Render() {
    auto speed = 3;
    glLoadIdentity();
    glTranslatef(-camera.x, -camera.y, 0.0f);

    rectangle(px, py, 0.1, 0.1);

    if (glfwGetKey(handle, GLFW_KEY_W)) { py += speed * GetDeltaTick(); }
    if (glfwGetKey(handle, GLFW_KEY_S)) { py -= speed * GetDeltaTick(); }
    if (glfwGetKey(handle, GLFW_KEY_D)) { px += speed * GetDeltaTick(); }
    if (glfwGetKey(handle, GLFW_KEY_A)) { px -= speed * GetDeltaTick(); }

    if (glfwGetMouseButton(handle, GLFW_MOUSE_BUTTON_1)) {
        double mouseX, mouseY;
        glfwGetCursorPos(handle, &mouseX, &mouseY);

        double visibleHeight = camera.height / camera.zoom;
        double halfHeight = visibleHeight / 2.0;
        double halfWidth = halfHeight * aspect;

        double worldX =
            (mouseX / width) * (2.0 * halfWidth)
            - halfWidth
            + camera.x;

        double worldY =
            halfHeight
            - (mouseY / height) * (2.0 * halfHeight)
            + camera.y;

        circle(worldX, worldY, 0.2);
    }
}

GLFWwindow* Game::GetWindow() const {
    return handle;
}
