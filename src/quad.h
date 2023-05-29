#pragma once

#include <GL/glew.h>

#include <utility>

using namespace std;

class Quad {

private:
    GLuint idVertexBuffer, idUvBuffer;

public:
    GLfloat x, y, w, h, u0, v0, u1, v1;

    Quad(Quad const&) = delete;
    Quad& operator=(Quad const&) = delete;

    Quad();
    Quad(GLfloat x, GLfloat y, GLfloat w, GLfloat h);
    Quad(GLfloat x, GLfloat y, GLfloat w, GLfloat h, GLfloat u0, GLfloat v0, GLfloat u1, GLfloat v1);
    ~Quad();

    void pushBuffer(bool vFlip, const pair<GLint, GLint>& displaySize) const;

    void setUVs(GLfloat u0, GLfloat v0, GLfloat u1, GLfloat v1);

    const GLuint& getVertexBufferId() const { return idVertexBuffer; }
    const GLuint& getUvBufferId() const { return idUvBuffer; }

};
