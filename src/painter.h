#pragma once

#include "quad.h"

struct Color {
    float r, g, b, a;
};

namespace painter {

    void initialize();

    void draw(Quad& quad, int texture, const Color& color);

}
