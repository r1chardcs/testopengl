//
// Created by dlllibstdntc on 10.09.2026.
//

#ifndef GAMEENGINE_DOOMSDAY_H
#define GAMEENGINE_DOOMSDAY_H

#include "RenderSystem.h"
#include "glfw3.h"
#include <windows.h>
#include <memory>
#include <functional>

#include "World.h"

#ifdef CreateWindow
#undef CreateWindow
#endif

class Window;

typedef std::function<void(Window*)> _OnWindowRenderCallback;
typedef std::function<void(Window*)> _OnWindowInitializeCallback;
typedef std::function<void(Window*, INT, INT)> _OnWindowResizeCallback;

class Window {
    FLOAT width, height;
    std::string title;
    GLFWwindow* handle;

    _OnWindowInitializeCallback InitializeCallback;
    _OnWindowRenderCallback RenderCallback;
    _OnWindowResizeCallback ResizeCallback;
public:
    Window(std::string title, FLOAT width, FLOAT height);

    void SetInitializeCallback(_OnWindowInitializeCallback callback);
    void SetResizeCallback(_OnWindowResizeCallback callback);
    void SetRenderCallback(_OnWindowRenderCallback callback);

    void Show();

    FLOAT GetWidth() const;
    FLOAT GetHeight() const;

    GLFWwindow* GetHandle() const;
};

class Doomsday {
    std::unique_ptr<RenderSystem> render_system = nullptr;
    std::unique_ptr<Window> window = nullptr;
    std::unique_ptr<World> world = nullptr;

    Doomsday() {}
    void CreateFields();
public:
    static Doomsday& GetInstance() {
        static Doomsday doomsday;
        return doomsday;
    }

    Window* GetWindow() const;

    void Run();
};


#endif //GAMEENGINE_DOOMSDAY_H
