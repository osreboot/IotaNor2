#include <GL/glew.h>
#include <iostream>

#include "quad.h"
#include "util.h"

using namespace std;

Quad::Quad() : Quad(0.0f, 0.0f, 0.0f, 0.0f) {}

Quad::Quad(GLfloat x, GLfloat y, GLfloat w, GLfloat h) :
        x(x), y(y), w(w), h(h) {

    idVertexBuffer = 0; // Silence Clang warnings!
    glGenBuffers(1, &idVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, idVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), nullptr, GL_STATIC_DRAW);
}

Quad::~Quad() {
    glDeleteBuffers(1, &idVertexBuffer);
}

void Quad::setBufferToDisplay(const pair<int, int>& displaySize) const {
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
    glBindBuffer(GL_ARRAY_BUFFER, idVertexBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(coordsVertexBuffer), coordsVertexBuffer);
}
