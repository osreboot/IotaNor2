#pragma once

#include "types.h"
#include "graphics/texture.h"
#include "graphics/quad.h"

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

    void draw(float delta, const Render& render, const Game& game);

};
