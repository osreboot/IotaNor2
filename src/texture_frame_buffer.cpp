
#include "texture_frame_buffer.h"
#include "display.h"

TextureFBO* TextureFBO::build(GLsizei w, GLsizei h) {
    GLuint idFrameBuffer = 0;
    glGenFramebuffers(1, &idFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, idFrameBuffer);

    return new TextureFBO(w, h, idFrameBuffer);
}

TextureFBO::TextureFBO(GLsizei w, GLsizei h, GLuint idFrameBuffer) :
        Texture(w, h, 0), idFrameBuffer(idFrameBuffer) {
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, getId(), 0);

    GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, drawBuffers);
}

TextureFBO::~TextureFBO() {
    glDeleteFramebuffers(1, &idFrameBuffer);
}

void TextureFBO::capture(bool clear, const std::function<void()>& func) const {
    glBindFramebuffer(GL_FRAMEBUFFER, idFrameBuffer);
    glViewport(0, 0, static_cast<GLsizei>(display::getSize().first), static_cast<GLsizei>(display::getSize().second));

    if(clear) glClear(GL_COLOR_BUFFER_BIT);

    func();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, static_cast<GLsizei>(display::getSize().first), static_cast<GLsizei>(display::getSize().second));
}
