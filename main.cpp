#include "Core/Window.h"
#include "Core/RenderEngine.h"

float size = 0.1;
float x = 20, y = 20;

#define MAP_H 40
#define MAP_W 40

int main() {
    glfwInit();
    Window window("yoka", 600, 600);
    RenderEngine RE;


    window.SetCallback([&RE](Window* h) {
        int w = h->width;
        int hh = h->height;

        float aspect = (float)w / (float)hh;

        float viewW, viewH;
        if (w >= hh) {
            viewH = MAP_H;
            viewW = MAP_H * aspect;
        } else {
            viewW = MAP_W;
            viewH = MAP_W / aspect;
        }

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-viewW/2, viewW/2, -viewH/2, viewH/2, -1, 1);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        printf("%d %d\n", h->width, h->height);

        if (h->GetKey(GLFW_KEY_W)) y += 0.1f;
        if (h->GetKey(GLFW_KEY_S)) y -= 0.1f;
        if (h->GetKey(GLFW_KEY_D)) x += 0.1f;
        if (h->GetKey(GLFW_KEY_A)) x -= 0.1f;

        RE.circle(x, y, 0.5f, COLOR_GREEN);
        RE.rectangle(x - size, y - size, x + size, y + size, COLOR_YELLOW);

        DrawText("Hello, World", 0, 0, 1, 1, 1, 0.05f);
    });
    window.Show();
    glfwTerminate();
    return 0;
}