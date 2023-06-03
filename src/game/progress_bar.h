#pragma once

#include "types.h"
#include "game/game.h"

class ProgressBar {

private:
    static const float W;

    Texture textureEnds, textureEndsBack;
    Texture textureCutout;
    Texture textureBlackGrow, textureWhiteGrow, textureBlackShrink, textureWhiteShrink;

    Quad quad, quadBlack, quadWhite;

    float visProgress;

public:

    ProgressBar();

    void render(float delta, const Render& render, const Game& game);

};
