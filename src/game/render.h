#pragma once

#include "game.h"
#include "../painter.h"
#include "../texture_frame_buffer.h"

class Render {

private:
    Shader shaderDefault, shaderRefract;

    TextureFBO frameBufferBackground;

    Texture textureTest, textureTileMask, textureTileRefC, textureTileRefL, textureTileRefUL;

    Quad quadScreen;

    float currentTileX = 0.0f, currentTileY = 0.0f;

public:
    static const Color WHITE;

    Render(Render const&) = delete;
    Render& operator=(Render const&) = delete;

    Render();
    ~Render();

    void render(const Game& game);

};
