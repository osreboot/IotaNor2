#pragma once

class Render;

#include "graphics/texture.h"
#include "graphics/quad.h"

// Represents the UI element for a styled progress bar that gradually steps towards the actual reference value. In this
// case, the element indicates the percentage of tiles that are illuminated. The elements visually changes to indicate
// whether the current objective is to make the board completely white or black.
class ProgressBar {

private:
    static const float WIDTH, ASPECT, POS_Y;

    Texture textureEnds, textureEndsBack;
    Texture textureCutout;
    Texture textureBlackGrow, textureWhiteGrow, textureBlackShrink, textureWhiteShrink;

    Quad quad, quadBlack, quadWhite;

    // The progress value that is currently being displayed (this is gradually stepped towards the actual value)
    float visProgress;

public:

    ProgressBar();

    // Updates and renders the progress bar
    void draw(float delta, const Render& render, const Game& game);

};
