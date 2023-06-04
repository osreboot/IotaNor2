#pragma once

#include "game/ui/progress_bar.h"
#include "graphics/font.h"
#include "graphics/painter.h"
#include "graphics/texture_frame_buffer.h"
#include "game/group.h"

class Render {

private:
    Shader shaderFire, shaderRefract, shaderMask;

    TextureFBO fboFire0, fboFire1;
    TextureFBO fboGameContent, fboRefractedContent;
    TextureFBO fboMask, fboMaskChannel0, fboMaskChannel1;

    Texture textureTest, textureSolid, textureNoise;
    Texture textureUiTileLit, textureUiTileSwap, textureUiCircle, textureUiQueueBox;
    Texture textureTileMask, textureTileRefC, textureTileRefL, textureTileRefUL;
    Texture textureBoardMask;

    Quad quadScreen, quadUiStageS, quadUiStageL, quadUiQueueBox;
    Quad quadTileInfection;
    Quad quadBoardMask;

    ProgressBar progressBar;
    Font font;

    float visFireIntensity;

    const Quad* currentTileQuad = nullptr;

    void render(float delta, Group& group, bool channelBoard);

public:
    static const Color WHITE;
    static const Color BLACK;

    float timer = 0.0f;

    Shader shaderDefault, shaderAlpha;

    Render(Render const&) = delete;
    Render& operator=(Render const&) = delete;

    explicit Render(Game& game);

    void render(float delta, Game& game);

};
