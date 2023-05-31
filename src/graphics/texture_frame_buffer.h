#pragma once

#include <functional>

#include "graphics/texture.h"

class TextureFBO : public Texture {

private:
    GLuint idFrameBuffer;

    TextureFBO(Coordw size, GLuint idFrameBuffer);

public:
    static TextureFBO build(Coordw size);

    TextureFBO(TextureFBO const&) = delete;
    TextureFBO& operator=(TextureFBO const&) = delete;

    ~TextureFBO();

    void capture(bool clear, const std::function<void()>& func) const;

};
