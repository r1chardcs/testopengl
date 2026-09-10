//
// Created by dlllibstdntc on 09.09.2026.
//

#include "Window.h"

Window::Window(const String &title, INT width, INT height) : title(title), width(width), height(height) {
    handle = glfwCreateWindow(width, height, title.c_str(), 0, 0);
}

void Window::SetCallback(std::function<void(Window *)> callback) {
    this->callback = callback;
}

void Window::Show() {
    glfwMakeContextCurrent(handle);
    glfwSetWindowUserPointer(handle, this);
    glfwSetFramebufferSizeCallback(handle, [](GLFWwindow* window, int width, int height) {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));

        if (!self)
            return;

        self->width = width;
        self->height = height;

        glViewport(0, 0, width, height);
    });
    while (true) {
        if (glfwWindowShouldClose(handle)) {
            break;
        }

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0, 0, 0, 0);

        if (callback)
            callback(this);

        glfwSwapBuffers(handle);
        glfwPollEvents();
    }
}

INT Window::GetKey(INT key) {
    return glfwGetKey(handle, key);
}
