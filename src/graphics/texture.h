#pragma once

#include "types.h"

class Texture {

private:
    GLuint idTexture;
    bool vFlip;

public:
    static Texture load(const char* path);

    Texture(Texture const&) = delete;
    Texture& operator=(Texture const&) = delete;

    Texture(Coordw size, GLint internalFormat, GLint format, void* pixels, bool freePixels, bool vFlip, bool mipmap);
    ~Texture();

    const GLuint& getId() const { return idTexture; }
    const bool getVFlip() const { return vFlip; }

};
