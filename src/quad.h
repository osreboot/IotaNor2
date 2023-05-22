#pragma once

#include <GL/glew.h>
#include <utility>

using namespace std;

class Quad {

private:
    GLuint idVertexBuffer;
    GLfloat coordsVertexBuffer[12];

public:
    Quad(float x, float y, float w, float h);
    ~Quad();

    void setBufferToDisplay(const pair<int, int>& displaySize);

    GLuint getId() const { return idVertexBuffer; }

};
