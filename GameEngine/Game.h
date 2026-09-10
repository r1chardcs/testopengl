//
// Created by dlllibstdntc on 10.09.2026.
//

#ifndef GAMEENGINE_GAME_H
#define GAMEENGINE_GAME_H

#include <windows.h>
#include <glfw3.h>

typedef DOUBLE TICK;

struct Camera {
    FLOAT x = 0.0f;
    FLOAT y = 0.0f;
    FLOAT zoom = 1.0;
    FLOAT height = 10.0;
};

class Game {
    TICK lastTime = 0;
    TICK deltaTick = 0;
    GLFWwindow* handle = nullptr;
    Camera camera;
public:
    void UpdateProjective(INT w, INT h);

    void Run();
    void Render();

    GLFWwindow* GetWindow() const;

    TICK GetDeltaTick() const {
        return deltaTick;
    }
};


#endif //GAMEENGINE_GAME_H
