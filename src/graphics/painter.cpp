
#include <GL/glew.h>

#include "graphics/display.h"
#include "graphics/painter.h"
#include "graphics/texture.h"

namespace painter {

    void initialize() {
        GLuint idVertexArray;
        glGenVertexArrays(1, &idVertexArray);
        glBindVertexArray(idVertexArray);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void draw(const Quad& quad, const Texture& texture, const Shader& shader, const Color& color) {
        glUseProgram(shader.getProgramId());

        quad.pushBuffer(texture.getVFlip(), display::getSize());

        GLint uniformLocationT0 = glGetUniformLocation(shader.getProgramId(), "textureBase");
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture.getId());
        glUniform1i(uniformLocationT0, 0);

        GLint uniformLocationTC = glGetUniformLocation(shader.getProgramId(), "textureColor");
        glUniform4f(uniformLocationTC, color.r, color.g, color.b, color.a);

        shader.applyCustomParameters();

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, quad.getVertexBufferId());
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, quad.getUvBufferId());
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
    }

}