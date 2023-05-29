#pragma once

#include <GL/glew.h>

class Texture {

private:
    GLuint idTexture;
    bool vFlip;

public:
    static Texture load(const char* path);

    Texture(Texture const&) = delete;
    Texture& operator=(Texture const&) = delete;

    Texture(GLsizei w, GLsizei h, const void* pixels, bool vFlip);
    ~Texture();

    const GLuint& getId() const { return idTexture; }
    const bool getVFlip() const { return vFlip; }

};
