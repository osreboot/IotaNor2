#pragma once

#include "types.h"

// Class that represents a simple rectangle (that can also optionally store texture UVs). Used as the canvas for
// rendering textures, colors, and invoking shader programs.
class Quad {

private:
    GLuint idVertexBuffer, idUvBuffer;

public:
    GLfloat x, y, w, h, u0, v0, u1, v1;

    // Disable copying. This is necessary because we need to keep this instance consistent with the IDs and specific
    // buffers reserved within the OpenGL context.
    Quad(Quad const&) = delete;
    Quad& operator=(Quad const&) = delete;

    Quad();
    Quad(GLfloat x, GLfloat y, GLfloat w, GLfloat h);
    Quad(GLfloat x, GLfloat y, GLfloat w, GLfloat h, GLfloat u0, GLfloat v0, GLfloat u1, GLfloat v1);
    ~Quad();

    // Binds the quad vertex data for rendering (after converting it from display coordinates to OpenGL coordinates,
    // e.g. from 0 -> 1920 to -1.0 -> 1.0).
    void pushBuffer(bool vFlip, const Coordw& displaySize) const;

    void setUVs(GLfloat u0, GLfloat v0, GLfloat u1, GLfloat v1);

    const GLuint& getVertexBufferId() const { return idVertexBuffer; }
    const GLuint& getUvBufferId() const { return idUvBuffer; }

};
