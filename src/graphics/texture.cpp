
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "graphics/texture.h"

Texture Texture::load(const char *path) {
    std::cout << "Loading texture at: " << path << std::endl;

    int w, h;
    unsigned char* image = stbi_load(path, &w, &h, nullptr, 4);
    return {{w, h}, GL_RGBA8, GL_RGBA, image, true, false, true};
}

Texture::Texture(Coordw size, GLint internalFormat, GLint format, void* pixels, bool freePixels, bool vFlip, bool mipmap) : vFlip(vFlip) {
    // Reserve the texture within the OpenGL context
    idTexture = 0; // Silence Clang warnings!
    glGenTextures(1, &idTexture);
    glBindTexture(GL_TEXTURE_2D, idTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, size.first, size.second, 0, format, GL_UNSIGNED_BYTE, pixels);

    // Generate mipmaps
    if (mipmap) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }

    // Set other filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    if (freePixels) stbi_image_free(pixels);
}

Texture::~Texture() {
    glDeleteTextures(1, &idTexture);
}
