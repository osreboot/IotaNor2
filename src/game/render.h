#pragma once

#include "game.h"
#include "../painter.h"
#include "../texture_frame_buffer.h"

class Render {

private:
    Shader shaderDefault, shaderRefract, shaderBackgroundFire;

    TextureFBO fboBackgroundFire0, fboBackgroundFire1, fboBackground;

    Texture textureTest, textureNoise, textureTileMask, textureTileRefC, textureTileRefL, textureTileRefUL;

    Quad quadScreen;

    float timer = 0.0f;
    float currentTileX = 0.0f, currentTileY = 0.0f;

public:
    static const Color WHITE;

    Render(Render const&) = delete;
    Render& operator=(Render const&) = delete;

    Render();
    ~Render();

    void render(float delta, const Game& game);

};
