#pragma once

#include <GL/glew.h>

#include <utility>

using namespace std;

class Quad {

private:
    GLuint idVertexBuffer;

public:
    GLfloat x, y, w, h;

    Quad(Quad const&) = delete;
    Quad& operator=(Quad const&) = delete;

    Quad();
    Quad(GLfloat x, GLfloat y, GLfloat w, GLfloat h);
    ~Quad();

    void setBufferToDisplay(const pair<GLint, GLint>& displaySize) const;

    const GLuint& getVertexBufferId() const { return idVertexBuffer; }

};
