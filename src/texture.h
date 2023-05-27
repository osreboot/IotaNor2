#pragma once

#include <GL/glew.h>

class Texture {

private:
    GLuint idTexture, idUvBuffer;

public:
    Texture(Texture const&) = delete;
    Texture& operator=(Texture const&) = delete;

    explicit Texture(const char* path);
    ~Texture();

    GLuint getId() const { return idTexture; }
    GLuint getUvBufferId() const { return idUvBuffer; }

};
