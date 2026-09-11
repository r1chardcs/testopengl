//
// Created by dlllibstdntc on 10.09.2026.
//

#include "RenderSystem.h"

#include <cmath>

#include "Doomsday.h"
#define STB_TRUETYPE_IMPLEMENTATION

#include <stb_truetype.h>

void BufferBuilder::FirstInitialize() {
    if (firstCall == FALSE) {
        if (coordinate_system == CoordinateSystem::WORLD) {
            glPushMatrix();

            glMatrixMode(GL_PROJECTION);
            glPushMatrix();
            glLoadIdentity();

            const FLOAT width =
                Doomsday::GetInstance().GetWindow()->GetWidth();

            const FLOAT height =
                Doomsday::GetInstance().GetWindow()->GetHeight();

            glOrtho(
                0,
                width,
                height,
                0,
                -1,
                1
            );

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
        }
        glBegin(mode);
        firstCall = TRUE;
    }
}

FLOAT BufferBuilder::Tx(FLOAT x) {
    return x;
}

FLOAT BufferBuilder::Ty(FLOAT y) {
    return y;
}

BufferBuilder & BufferBuilder::Color3(FLOAT r, FLOAT g, FLOAT b) {
    FirstInitialize();
    glColor3f(r, g, b);

    return *this;
}

BufferBuilder & BufferBuilder::Color4(FLOAT r, FLOAT g, FLOAT b, FLOAT a) {
    FirstInitialize();
    glColor4f(r, g, b, a);

    return *this;
}

BufferBuilder & BufferBuilder::Vertex2(FLOAT x, FLOAT y) {
    FirstInitialize();
    glVertex2f(Tx(x), Ty(y));

    return *this;
}

BufferBuilder & BufferBuilder::Vertex3(FLOAT x, FLOAT y, FLOAT z) {
    FirstInitialize();
    glVertex3f(Tx(x), Ty(y), z);

    return *this;
}

BufferBuilder & BufferBuilder::Rect2x2(FLOAT x, FLOAT y, FLOAT width, FLOAT height) {
    FirstInitialize();

    Vertex2(x - width, y - height);
    Vertex2(x + width, y - height);
    Vertex2(x + width, y + height);
    Vertex2(x - width, y + height);

    return *this;
}

BufferBuilder & BufferBuilder::Line2(FLOAT minX, FLOAT maxX, FLOAT width)
{
    FirstInitialize();

    glLineWidth(width);
    glVertex2f(minX, maxX);

    return *this;
}

BufferBuilder BufferBuilder::Circle2x2(FLOAT x, FLOAT y, FLOAT rad) {
    constexpr FLOAT attempt = 360;

    for (unsigned i = 0; i < attempt; i++) {
        FLOAT angle = 2 * 3.14 * i / attempt;
        FLOAT px = x + cosf(angle) * rad;
        FLOAT py = y + sinf(angle) * rad;

        glVertex2f(px, py);
    }

    return *this;
}

BufferBuilder & BufferBuilder::End() {
    if (coordinate_system == CoordinateSystem::WORLD) {
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();

        glMatrixMode(GL_PROJECTION);
        glPopMatrix();

        glMatrixMode(GL_MODELVIEW);
    }
    glEnd();
    return *this;
}

void RenderSystem::Setup(Window *window) {
    camera = std::make_unique<Camera>();
    camera->x = 0;
    camera->y = 0;
    camera->height = 100;
    camera->zoom = 1;

    Resize(window,
        static_cast<INT>(window->GetWidth()),
        static_cast<INT>(window->GetHeight()));
}

void RenderSystem::Resize(Window *window, INT w, INT h) const {
    glViewport(0, 0, w, h);
    UpdateProjective(w, h);
}

void RenderSystem::Update(Window *window) {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor4f(0, 0, 0, 1);

    auto now = glfwGetTime();
    deltaTime = now - lastTime;
    lastTime = now;

    glLoadIdentity();
    glTranslatef(-camera->x, -camera->y, 0);
}

TICK RenderSystem::GetDeltaTime() const {
    return deltaTime;
}

void RenderSystem::UpdateProjective(INT w, INT h) const {
    const auto aspect = static_cast<float>(w) / h;
    const auto visibleHeight = camera->height / camera->zoom;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    const auto halfHeight = visibleHeight / 2;
    const auto halfWidth = halfHeight * aspect;

    glOrtho(-halfWidth, halfWidth, -halfHeight, halfHeight, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

Camera * RenderSystem::GetCamera() const {
    return camera.get();
}

FLOAT RenderSystem::ToWorldX(FLOAT x) const {
    const FLOAT width =
        Doomsday::GetInstance().GetWindow()->GetWidth();

    const FLOAT height =
        Doomsday::GetInstance().GetWindow()->GetHeight();

    const FLOAT aspect = width / height;

    const FLOAT visibleHeight =
        camera->height / camera->zoom;

    const FLOAT visibleWidth =
        visibleHeight * aspect;

    return ((x / width) - 0.5f) * visibleWidth + camera->x;
}

FLOAT RenderSystem::ToWorldY(FLOAT y) const {
    const FLOAT width =
        Doomsday::GetInstance().GetWindow()->GetWidth();

    const FLOAT height =
        Doomsday::GetInstance().GetWindow()->GetHeight();

    const FLOAT aspect = width / height;

    const FLOAT visibleHeight =
        camera->height / camera->zoom;

    return (0.5f - y / height) * visibleHeight + camera->y;
}

BufferBuilder RenderSystem::Buffer(INT mode, CoordinateSystem coordinate_system) {
    BufferBuilder bb(this, mode, coordinate_system);
    return bb;
}

std::shared_ptr<Font> RenderSystem::LoadFont(const char *path, FLOAT _size) {
    auto font = std::make_shared<Font>();

    FILE* f = fopen(path, "rb");
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    std::vector<unsigned char> ttfBuffer(size);
    fread(ttfBuffer.data(), 1, size, f);
    fclose(f);

    std::vector<unsigned char> alphaOnly(font->atlasW * font->atlasH);

    stbtt_BakeFontBitmap(ttfBuffer.data(), 0, _size,
                          alphaOnly.data(), font->atlasW, font->atlasH,
                          32, 96, font->cdata);

    std::vector<unsigned char> la(font->atlasW * font->atlasH * 2);
    for (int i = 0; i < font->atlasW * font->atlasH; i++) {
        la[i * 2 + 0] = 255;
        la[i * 2 + 1] = alphaOnly[i];
    }

    glGenTextures(1, &font->fontTexture);
    glBindTexture(GL_TEXTURE_2D, font->fontTexture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, font->atlasW, font->atlasH, 0,
                 GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, la.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return font;
}

void RenderSystem::RenderText(Font *font, const char *text, float px, float py, float r, float g, float b,
    float scale) {
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, font->fontTexture);
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
            stbtt_GetBakedQuad(font->cdata, font->atlasW, font->atlasH, *text - 32, &x, &y, &q, 1);

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

FLOAT RenderSystem::GetTextWidth(Font *font, const char *text, FLOAT scale) const {
    float x = 0, y = 0;
    stbtt_aligned_quad q;

    while (*text) {
        if (*text >= 32 && *text < 128) {
            stbtt_GetBakedQuad(font->cdata, font->atlasW, font->atlasH, *text - 32, &x, &y, &q, 1);
        }
        text++;
    }

    return x * scale;
}

