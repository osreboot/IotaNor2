#pragma once

#include "game.h"
#include "../painter.h"
#include "../texture_frame_buffer.h"

namespace render {

    const Color WHITE = {1.0f, 1.0f, 1.0f, 1.0f};

    static Shader* shaderDefault;
    static TextureFBO* fbo0;

    void initialize();
    void render(Game& game);
    void cleanup();

}
