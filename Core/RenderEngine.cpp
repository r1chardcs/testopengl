//
// Created by dlllibstdntc on 09.09.2026.
//

#include "RenderEngine.h"

#include <cmath>

void RenderEngine::rectangle(FLOAT minX, FLOAT minY, FLOAT maxX, FLOAT maxY, const Color &color) {
    glBegin(GL_QUADS);
    glColor4f(color.r, color.g, color.b, color.a);
    glVertex2f(minX, minY);
    glVertex2f(maxX, minY);
    glVertex2f(maxX, maxY);
    glVertex2f(minX, maxY);
    glEnd();
}

void RenderEngine::circle(FLOAT x, FLOAT y, FLOAT rad, const Color &color) {
    glColor4f(color.r, color.g, color.b, color.a);
    FLOAT attempt = 360;

    glBegin(GL_LINE_LOOP);
    for (INT i = 0; i < attempt; i++) {
        FLOAT angle = 2 * 3.14 * i / attempt;

        FLOAT px = x + sinf(angle) * rad;
        FLOAT py = y + cosf(angle) * rad;

        glVertex2f(px, py);
    }
    glEnd();
}
