#include <GL/glew.h>
#include "quad.h"
#include "util.h"

Quad::Quad(float x, float y, float w, float h) {
    GLfloat coordsVertexBufferTemp[] = {
            x, y,
            x + w, y,
            x, y + h,
            x, y + h,
            x + w, y + h,
            x + w, y,
    };
    for(int i = 0; i < sizeof(coordsVertexBuffer) / sizeof(GLfloat); i++){
        coordsVertexBuffer[i] = coordsVertexBufferTemp[i];
    }

    glGenBuffers(1, &idVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, idVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(coordsVertexBuffer), coordsVertexBuffer, GL_STATIC_DRAW);
}

Quad::~Quad() {
    glDeleteBuffers(1, &idVertexBuffer);
}

void Quad::setBufferToDisplay(const pair<int, int>& displaySize) {
    GLfloat coordsVertexBufferTemp[12];
    for(int i = 0; i < sizeof(coordsVertexBuffer) / sizeof(GLfloat); i += 2){
        coordsVertexBufferTemp[i] = map(coordsVertexBuffer[i],
                                        0.0f, static_cast<float>(displaySize.first), -1.0f, 1.0f);
        coordsVertexBufferTemp[i + 1] = map(coordsVertexBuffer[i + 1],
                                            0.0f, static_cast<float>(displaySize.second), 1.0f, -1.0f);
    }
    glBindBuffer(GL_ARRAY_BUFFER, idVertexBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(coordsVertexBufferTemp), coordsVertexBufferTemp);
}
