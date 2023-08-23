#pragma once

#include "GL/glew.h"

#include "vec2i.h"

// Represents an image (to be drawn to a quad)
class Texture {

private:
    GLuint idTexture;
    bool vFlip; // Whether to flip the texture vertically (primarily used to fix FBO orientation)

public:
    // Loads the texture at 'path', including the file name and extension
    static Texture load(const char* path);

    // Disable copying. This is necessary because we need to keep this instance consistent with the texture ID reserved
    // within the OpenGL context.
    Texture(Texture const&) = delete;
    Texture& operator=(Texture const&) = delete;

    // Generates a texture instance from the data 'pixels', with pixel resolution 'size', stored as format
    // 'internalFormat', from the origin format 'format'. Other parameters:
    // - 'freePixels' whether to free the pixel data after it is processed
    // - 'vFlip' whether the texture should be flipped vertically
    // - 'mipmap' whether to generate mipmaps for the texture
    Texture(vec2i size, GLint internalFormat, GLint format, void* pixels, bool freePixels, bool vFlip, bool mipmap);
    ~Texture();

    const GLuint& getId() const { return idTexture; }
    const bool getVFlip() const { return vFlip; }

};
