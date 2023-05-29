#pragma once

#include <functional>

#include "texture.h"

class TextureFBO : public Texture {

private:
    GLuint idFrameBuffer;

    TextureFBO(GLsizei w, GLsizei h, GLuint idFrameBuffer);

public:
    static TextureFBO build(GLsizei w, GLsizei h);

    TextureFBO(TextureFBO const&) = delete;
    TextureFBO& operator=(TextureFBO const&) = delete;

    ~TextureFBO();

    void capture(bool clear, const std::function<void()>& func) const;

};
