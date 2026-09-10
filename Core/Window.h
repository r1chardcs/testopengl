//
// Created by dlllibstdntc on 09.09.2026.
//

#ifndef YOKA_WINDOW_H
#define YOKA_WINDOW_H

#include "../include/glfw3.h"
#include <windows.h>
#include <string>
#include <functional>

typedef std::string String;

class Window {
    GLFWwindow* handle = nullptr;
    String title;
    std::function<void((Window*))> callback;
public:
    INT width = 0;
    INT height = 0;
    Window(const String& title, INT width, INT height);

    void SetCallback(std::function<void(Window*)> callback);
    void Show();

    INT GetKey(INT key);
};


#endif //YOKA_WINDOW_H
