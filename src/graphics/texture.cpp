
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "graphics/texture.h"

Texture Texture::load(const char *path) {
    std::cout << "Loading texture at: " << path << std::endl;

    int w, h;
    unsigned char* image = stbi_load(path, &w, &h, nullptr, 4);
    return {{w, h}, image, false, true};
}

Texture::Texture(Coordw size, const void* pixels, bool vFlip, bool mipmap) : vFlip(vFlip) {
    idTexture = 0; // Silence Clang warnings!
    glGenTextures(1, &idTexture);
    glBindTexture(GL_TEXTURE_2D, idTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size.first, size.second, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    if (mipmap) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

Texture::~Texture() {
    glDeleteTextures(1, &idTexture);
}
