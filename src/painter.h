#pragma once

#include "quad.h"
#include "texture.h"

struct Color {
    GLfloat r, g, b, a;
};

namespace painter {

    void initialize();

    void draw(const Quad& quad, const Texture& texture, const Color& color);

}
