//
// Created by dlllibstdntc on 10.09.2026.
//

#include "Doomsday.h"

Window::Window(std::string title, FLOAT width, FLOAT height) : title(std::move(title)), width(width), height(height) {
    handle = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
}

void Window::SetInitializeCallback(_OnWindowInitializeCallback callback) {
    InitializeCallback = callback;
}

void Window::SetResizeCallback(_OnWindowResizeCallback callback) {
    ResizeCallback = callback;
}

void Window::SetRenderCallback(_OnWindowRenderCallback callback) {
    RenderCallback = callback;
}

void Window::Show() {
    glfwMakeContextCurrent(handle);
    glfwSetWindowUserPointer(handle, this);

    glfwSetFramebufferSizeCallback(handle, [](GLFWwindow* handle, INT w, INT h) {
        if (auto self = static_cast<Window *>(glfwGetWindowUserPointer(handle))) {
            if (self->ResizeCallback)
                self->ResizeCallback(self, w, h);

            self->width = w;
            self->height = h;
        }
    });

    if (InitializeCallback) {
        InitializeCallback(this);
    }

    while (TRUE) {
        if (glfwWindowShouldClose(handle))
            break;

        if (RenderCallback)
            RenderCallback(this);

        glfwSwapBuffers(handle);
        glfwPollEvents();
    }
}

FLOAT Window::GetWidth() const {
    return width;
}

FLOAT Window::GetHeight() const {
    return height;
}

GLFWwindow * Window::GetHandle() const {
    return handle;
}

void Doomsday::CreateFields() {
    render_system = std::make_unique<RenderSystem>();
    window = std::make_unique<Window>("Doomsday", 1920, 1080);
    world = std::make_unique<World>();
}

Window * Doomsday::GetWindow() const {
    return window.get();
}

auto lastTime = glfwGetTime();

void Doomsday::Run() {
    CreateFields();

    window->SetInitializeCallback([this](auto w) {
        render_system->Setup(w);
    });

    window->SetResizeCallback([this](auto _window, auto w, auto h) {
        render_system->Resize(_window, w, h);
    });



    window->SetRenderCallback([this](auto w) {
        render_system->Update(w);


        world->RenderWorld(render_system.get());
    });

    window->Show();
}
