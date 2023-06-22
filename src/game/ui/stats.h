#pragma once

#include "game/render.h"
#include "graphics/font.h"
#include "types.h"

// Class responsible for tracking and rendering game statistics text (score, time elapsed, etc) and some other text
class Stats {

private:
    static const float POS_X, POS_Y;
    static const float FONT_OFFSET_Y;

    float timerTotal, timerStage;
    int piecesTotal, piecesStage;

public:
    Stats();

    // These functions should be called when game events happen to cycle/record various statistics
    void onGameStart();
    void onStageAdvance();
    void onPiecePlace();

    // Advances timers
    void update(float delta, const Game& game);

    // Renders text to the screen
    void draw(const Render& render, const Game& game, Font& font);

};
