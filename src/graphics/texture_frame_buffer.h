#pragma once

#include <functional>

#include "graphics/texture.h"
#include "vec2i.h"

// Represents an automatically managed frame buffer object
class TextureFBO : public Texture {

private:
    GLuint idFrameBuffer;

    TextureFBO(vec2i size, GLuint idFrameBuffer);

public:
    // Reserves a frame buffer of pixel resolution 'size'
    static TextureFBO build(vec2i size);

    // Disable copying. This is necessary because we need to keep this instance consistent with the texture ID reserved
    // within the OpenGL context.
    TextureFBO(TextureFBO const&) = delete;
    TextureFBO& operator=(TextureFBO const&) = delete;

    ~TextureFBO();

    // Renders all content drawn within 'func' to the frame buffer. If 'clear' is set, clear existing content prior.
    void capture(bool clear, const std::function<void()>& func) const;

};
