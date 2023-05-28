#pragma once

#include "quad.h"
#include "texture.h"
#include "shader.h"

struct Color {
    GLfloat r, g, b, a;
};

namespace painter {

    void initialize();

    void draw(const Quad& quad, const Texture& texture, const Shader& shader, const Color& color);

}
