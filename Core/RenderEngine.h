//
// Created by dlllibstdntc on 09.09.2026.
//

#ifndef YOKA_RENDERENGINE_H
#define YOKA_RENDERENGINE_H
#include "Window.h"

struct Color {
    float r,g,b,a;
};

#define MAKE_COLOR(r, g, b, a) Color {r, g, b, a}

#define COLOR_RED MAKE_COLOR(1,0,0,1)
#define COLOR_GREEN MAKE_COLOR(0,1,0,1)
#define COLOR_BLUE MAKE_COLOR(0,0,1,1)
#define COLOR_YELLOW MAKE_COLOR(1,1,0,1)

struct Glyph {
    float u0, v0;
    float u1, v1;

    int width;
    int height;

    int bearingX;
    int bearingY;

    int advance;
};
#undef DrawText

void LoadFont(const char* path, float pixelHeight);
void DrawText(const char* text, float px, float py, float r, float g, float b, float scale);

struct RenderEngine {
    void rectangle(FLOAT minX, FLOAT minY, FLOAT maxX, FLOAT maxY,
        const Color &color);

    void circle(FLOAT x, FLOAT y, FLOAT rad, const Color &color);
};


#endif //YOKA_RENDERENGINE_H
