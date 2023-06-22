#pragma once

#include "graphics/quad.h"
#include "graphics/shader.h"
#include "graphics/texture.h"

// Represents a RGBA color with value ranges of 0.0 to 1.0
struct Color {
    GLfloat r, g, b, a;
};

namespace painter {

    // Configures the OpenGL context for rendering
    void initialize();

    // Renders a textured, colorized quad to the screen
    void draw(const Quad& quad, const Texture& texture, const Shader& shader, const Color& color);

}
