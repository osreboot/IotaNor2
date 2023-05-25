#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../lib/stb/stb_image.h"

Texture::Texture(const char* path) {
    int w, h;
    unsigned char* image = stbi_load(path, &w, &h, nullptr, 4);

    idTexture = 0; // Silence Clang warnings!
    glGenTextures(1, &idTexture);
    glBindTexture(GL_TEXTURE_2D, idTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    GLfloat coordsUvBuffer[] = {
            0.0f, 0.0f,
            1.0f, 0.0f,
            0.0f, 1.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
    };

    idUvBuffer = 0; // Silence Clang warnings!
    glGenBuffers(1, &idUvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, idUvBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(coordsUvBuffer), coordsUvBuffer, GL_STATIC_DRAW);
}

Texture::~Texture() {
    glDeleteTextures(1, &idTexture);
    glDeleteBuffers(1, &idUvBuffer);
}
