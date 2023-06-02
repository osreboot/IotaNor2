#pragma once

#include "game/game.h"
#include "graphics/painter.h"
#include "graphics/texture_frame_buffer.h"

class Render {

private:
    Shader shaderDefault, shaderFire, shaderRefract;

    TextureFBO fboFire0, fboFire1;
    TextureFBO fboGameContent, fboRefractedContent;

    Texture textureTest, textureNoise;
    Texture textureUiTileLit, textureUiTileSwap, textureUiCircle;
    Texture textureTileMask, textureTileRefC, textureTileRefL, textureTileRefUL;

    Quad quadScreen, quadUiStageS, quadUiStageL;
    Quad quadTileInfection;

    float timer = 0.0f;

    float visFireIntensity;

    const Quad* currentTileQuad = nullptr;

    void render(Group& group);

public:
    static const Color WHITE;
    static const Color BLACK;

    Render(Render const&) = delete;
    Render& operator=(Render const&) = delete;

    explicit Render(Game& game);

    void render(float delta, Game& game);

};
