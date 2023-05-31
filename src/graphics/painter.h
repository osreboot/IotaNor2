#pragma once

#include "graphics/quad.h"
#include "graphics/shader.h"
#include "graphics/texture.h"

struct Color {
    GLfloat r, g, b, a;
};

namespace painter {

    void initialize();

    void draw(const Quad& quad, const Texture& texture, const Shader& shader, const Color& color);

}
