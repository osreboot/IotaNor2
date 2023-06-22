
#include <GL/glew.h>

#include "graphics/display.h"
#include "graphics/painter.h"
#include "graphics/texture.h"

namespace painter {

    void initialize() {
        // Reserve a vertex array for quad rendering
        GLuint idVertexArray;
        glGenVertexArrays(1, &idVertexArray);
        glBindVertexArray(idVertexArray);

        // Enable alpha blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void draw(const Quad& quad, const Texture& texture, const Shader& shader, const Color& color) {
        glUseProgram(shader.getProgramId());

        quad.pushBuffer(texture.getVFlip(), display::getSize());

        // Send 'texture' to the shader program
        GLint uniformLocationT0 = glGetUniformLocation(shader.getProgramId(), "textureBase");
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture.getId());
        glUniform1i(uniformLocationT0, 0);

        // Send 'color' to the shader program
        GLint uniformLocationTC = glGetUniformLocation(shader.getProgramId(), "textureColor");
        glUniform4f(uniformLocationTC, color.r, color.g, color.b, color.a);

        // Send other shader-specific parameters to the shader program
        shader.applyCustomParameters();

        // Bind the quad vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, quad.getVertexBufferId());
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        // Bind the quad UVs
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, quad.getUvBufferId());
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        // Draw the quad
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Unbind quad vertices and UVs
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
    }

}
