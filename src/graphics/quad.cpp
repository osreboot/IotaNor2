
#include <iostream>

#include "graphics/quad.h"
#include "util.h"

Quad::Quad() : Quad(0.0f, 0.0f, 0.0f, 0.0f) {}

Quad::Quad(GLfloat x, GLfloat y, GLfloat w, GLfloat h) : Quad(x, y, w, h, 0.0f, 0.0f, 1.0f, 1.0f) {}

Quad::Quad(GLfloat x, GLfloat y, GLfloat w, GLfloat h, GLfloat u0, GLfloat v0, GLfloat u1, GLfloat v1) :
        x(x), y(y), w(w), h(h), u0(u0), v0(v0), u1(u1), v1(v1) {

    // Reserve space for the vertices within OpenGL
    idVertexBuffer = 0; // Silence Clang warnings!
    glGenBuffers(1, &idVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, idVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), nullptr, GL_STATIC_DRAW);

    // Reserve space for the UVs within OpenGL
    idUvBuffer = 0; // Silence Clang warnings!
    glGenBuffers(1, &idUvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, idUvBuffer);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), nullptr, GL_STATIC_DRAW);
}

Quad::~Quad() {
    glDeleteBuffers(1, &idVertexBuffer);
    glDeleteBuffers(1, &idUvBuffer);
}

void Quad::pushBuffer(bool vFlip, const Coordw& displaySize) const {
    // Convert vertices from display coordinates to OpenGL coordinates (e.g. from 0 -> 1920 to -1.0 -> 1.0)
    GLfloat coordsVertexBuffer[] = {
            x, y,
            x + w, y,
            x, y + h,
            x, y + h,
            x + w, y + h,
            x + w, y,
    };
    for(int i = 0; i < sizeof(coordsVertexBuffer) / sizeof(GLfloat); i += 2){
        coordsVertexBuffer[i] = map(coordsVertexBuffer[i], 0.0f, static_cast<float>(displaySize.first), -1.0f, 1.0f);
        coordsVertexBuffer[i + 1] = map(coordsVertexBuffer[i + 1], 0.0f, static_cast<float>(displaySize.second), 1.0f, -1.0f);
    }

    // Update OpenGL vertex buffer with converted data
    glBindBuffer(GL_ARRAY_BUFFER, idVertexBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(coordsVertexBuffer), coordsVertexBuffer);

    // Update OpenGL UV buffer with current UVs
    GLfloat coordsUvBuffer[] = {
            u0, v0,
            u1, v0,
            u0, vFlip ? -v1 : v1,
            u0, vFlip ? -v1 : v1,
            u1, vFlip ? -v1 : v1,
            u1, v0,
    };
    glBindBuffer(GL_ARRAY_BUFFER, idUvBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(coordsUvBuffer), coordsUvBuffer);
}

void Quad::setUVs(GLfloat u0Arg, GLfloat v0Arg, GLfloat u1Arg, GLfloat v1Arg) {
    u0 = u0Arg;
    v0 = v0Arg;
    u1 = u1Arg;
    v1 = v1Arg;
}
