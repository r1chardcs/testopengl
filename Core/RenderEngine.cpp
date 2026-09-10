//
// Created by dlllibstdntc on 09.09.2026.
//

#include "RenderEngine.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "../include/stb_truetype.h"

#include <cmath>

stbtt_bakedchar cdata[96];
GLuint fontTexture;
const int atlasW = 512;
const int atlasH = 512;

void LoadFont(const char* path, float pixelHeight) {
    FILE* f = fopen(path, "rb");
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    std::vector<unsigned char> ttfBuffer(size);
    fread(ttfBuffer.data(), 1, size, f);
    fclose(f);

    std::vector<unsigned char> alphaOnly(atlasW * atlasH);

    stbtt_BakeFontBitmap(ttfBuffer.data(), 0, pixelHeight,
                          alphaOnly.data(), atlasW, atlasH,
                          32, 96, cdata);

    // Разворачиваем в LUMINANCE_ALPHA: L всегда 255, A = coverage
    std::vector<unsigned char> la(atlasW * atlasH * 2);
    for (int i = 0; i < atlasW * atlasH; i++) {
        la[i * 2 + 0] = 255;
        la[i * 2 + 1] = alphaOnly[i];
    }

    glGenTextures(1, &fontTexture);
    glBindTexture(GL_TEXTURE_2D, fontTexture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, atlasW, atlasH, 0,
                 GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, la.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void DrawText(const char* text, float px, float py, float r, float g, float b, float scale) {
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, fontTexture);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glColor4f(r, g, b, 1.0f);

    glPushMatrix();
    glTranslatef(px, py, 0);
    glScalef(scale, -scale, 1.0f);

    float x = 0, y = 0;
    glBegin(GL_QUADS);
    while (*text) {
        if (*text >= 32 && *text < 128) {
            stbtt_aligned_quad q;
            stbtt_GetBakedQuad(cdata, atlasW, atlasH, *text - 32, &x, &y, &q, 1);

            glTexCoord2f(q.s0, q.t0); glVertex2f(q.x0, q.y0);
            glTexCoord2f(q.s1, q.t0); glVertex2f(q.x1, q.y0);
            glTexCoord2f(q.s1, q.t1); glVertex2f(q.x1, q.y1);
            glTexCoord2f(q.s0, q.t1); glVertex2f(q.x0, q.y1);
        }
        text++;
    }
    glEnd();
    glPopMatrix();

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

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