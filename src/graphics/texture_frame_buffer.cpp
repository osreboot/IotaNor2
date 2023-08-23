
#include "graphics/display.h"
#include "graphics/texture_frame_buffer.h"

TextureFBO TextureFBO::build(vec2i size) {
    // Reserve the frame buffer within the OpenGL context
    GLuint idFrameBuffer = 0;
    glGenFramebuffers(1, &idFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, idFrameBuffer);

    return {size, idFrameBuffer};
}

TextureFBO::TextureFBO(vec2i size, GLuint idFrameBuffer) :
        Texture(size, GL_RGBA8, GL_RGBA, 0, false, true, false), idFrameBuffer(idFrameBuffer) {
    // Configure other frame buffer attributes
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, getId(), 0);

    GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, drawBuffers);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

TextureFBO::~TextureFBO() {
    glDeleteFramebuffers(1, &idFrameBuffer);
}

void TextureFBO::capture(bool clear, const std::function<void()>& func) const {
    // Bind the frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, idFrameBuffer);
    glViewport(0, 0, display::getSize().x, display::getSize().y);

    if(clear) glClear(GL_COLOR_BUFFER_BIT);

    func(); // Invoke the function to render the user-defined content

    // Unbind the frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, display::getSize().x, display::getSize().y);
}
