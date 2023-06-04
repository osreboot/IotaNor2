#pragma once

#include "game/render.h"
#include "graphics/font.h"
#include "types.h"

class Stats {

private:
    float timerTotal, timerStage;
    int piecesTotal, piecesStage;

public:
    Stats();

    void onGameStart();
    void onStageAdvance();
    void onPiecePlace();

    void update(float delta, const Game& game);
    void render(const Render& render, const Game& game, Font& font);

};
