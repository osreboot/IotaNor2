#pragma once

#include <GL/glew.h>

class Texture {

private:
    GLuint idTexture, idUvBuffer;

public:
    static Texture load(const char* path);

    Texture(Texture const&) = delete;
    Texture& operator=(Texture const&) = delete;

    Texture(GLsizei w, GLsizei h, const void* pixels);
    ~Texture();

    const GLuint& getId() const { return idTexture; }
    const GLuint& getUvBufferId() const { return idUvBuffer; }

};
