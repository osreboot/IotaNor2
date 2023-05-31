#pragma once

#include "game.h"
#include "../painter.h"
#include "../texture_frame_buffer.h"

class Render {

private:
    Shader shaderDefault, shaderFire, shaderRefract;

    TextureFBO fboFire0, fboFire1;
    TextureFBO fboGameContent, fboRefractedContent;

    Texture textureTest, textureNoise;
    Texture textureTileMask, textureTileRefC, textureTileRefL, textureTileRefUL;

    Quad quadScreen;

    float timer = 0.0f;

    const Quad* currentTileQuad = nullptr;

public:
    static const Color WHITE;
    static const Color BLACK;

    Render(Render const&) = delete;
    Render& operator=(Render const&) = delete;

    Render();
    ~Render();

    void render(float delta, const Game& game);

};
