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
    Texture textureUiTileSwap, textureUiTileSwapSmall, textureUiCircle, textureUiQueueBox, textureUiAudioMuted;
    Texture textureTileMask, textureTileRefC, textureTileRefL, textureTileRefUL, textureTileLit;
    Texture textureBoardMask;

    Quad quadScreen;
    Quad quadUiStageS, quadUiStageL, quadUiQueueBox, quadUiAudioMuted;
    Quad quadTileInfection;
    Quad quadBoardMask;

    ProgressBar progressBar;
    Font font;

    float visFireIntensity;

    bool refractSendTextures = false;
    const Quad* refractQuad = nullptr;

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
