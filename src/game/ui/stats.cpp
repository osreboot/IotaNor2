
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <cmath>

#include "game/game.h"
#include "game/ui/stats.h"
#include "graphics/display.h"

Stats::Stats() :
        timerTotal(0.0f),
        timerStage(0.0f),
        piecesTotal(0),
        piecesStage(0) {}

void Stats::onGameStart() {
    timerTotal = 0.0f;
    timerStage = 0.0f;
    piecesTotal = 0;
    piecesStage = 0;
}

void Stats::onStageAdvance() {
    timerStage = 0.0f;

    piecesTotal += piecesStage;
    piecesStage = 0;
}

void Stats::onPiecePlace() {
    piecesStage++;
}

void Stats::update(float delta, const Game& game) {
    if (!game.frozen) {
        timerTotal += delta;
        timerStage += delta;
    }
}

void Stats::render(const Render& render, const Game& game, Font& font) {
    if (game.frozen) {

        // Display the title cards
        std::string strTitle = "iota nor";
        Coordf locationStrTitle = {Game::getOriginQueue(0).first - (font.getSize(strTitle.c_str()).first / 2.0f),
                                   Game::getOriginQueue(0).second + (Tile::SIZE / 2.0f) + 6.0f};
        font.render(render, strTitle.c_str(), locationStrTitle, Render::BLACK);

        std::string strCredit = "by Calvin Weaver";
        Coordf locationStrCredit = {Game::getOriginHold().first - (font.getSize(strCredit.c_str()).first / 2.0f),
                                    Game::getOriginHold().second + (Tile::SIZE / 2.0f) + 6.0f};
        font.render(render, strCredit.c_str(), locationStrCredit, Render::BLACK);

        // Check if another game is ready to start
        if (game.timerFrozenGrace <= 0.0f) {
            std::string strStart = "click to start";
            Coordf locationStrStart = {(static_cast<float>(display::getSize().first) / 2.0f) - (font.getSize(strStart.c_str()).first / 2.0f),
                                       static_cast<float>(display::getSize().second) - 48.0f + 6.0f};
            font.render(render, strStart.c_str(), locationStrStart, Render::WHITE);
        }

        // Check if the player just finished a game
        if (fmod(render.timer, 1.0) <= 0.5f && timerTotal > 0.0f) {
            std::stringstream stream;
            stream << std::fixed << std::setprecision(2) << timerTotal;
            std::string strFinish = stream.str() + "  seconds            " + std::to_string(piecesTotal) + "  pieces";
            Coordf locationStrFinish = {(static_cast<float>(display::getSize().first) / 2.0f) - (font.getSize(strFinish.c_str()).first / 2.0f), 48.0f + 6.0f};
            font.render(render, strFinish.c_str(), locationStrFinish, Render::WHITE);
        }
    } else {
        float yTextUp = 48.0f + 6.0f;
        float yTextDown = static_cast<float>(display::getSize().second) - 48.0f + 6.0f;
        float xTextLeft = (static_cast<float>(display::getSize().first) / 2.0f)
                          - (static_cast<float>(Game::BOARD_DIM) * Tile::SIZE / 2.0f) + 88.0f;
        float xTextRight = (static_cast<float>(display::getSize().first) / 2.0f)
                           + (static_cast<float>(Game::BOARD_DIM) * Tile::SIZE / 2.0f) - 88.0f - 104.0f;

        std::stringstream stream;

        stream << std::fixed << std::setprecision(2) << timerTotal;
        std::string strTimeTotal = "total:  " + stream.str();
        font.render(render, strTimeTotal.c_str(), {xTextLeft, yTextUp}, Render::WHITE);

        stream.str("");
        stream.clear();
        stream << std::fixed << std::setprecision(2) << timerStage;
        std::string strTimeStage = "current:  " + stream.str();
        font.render(render, strTimeStage.c_str(), {xTextRight, yTextUp}, Render::WHITE);

        std::string strStage = "stage:  " + std::to_string(game.stage + 1);
        font.render(render, strStage.c_str(), {xTextLeft, yTextDown}, Render::WHITE);

        std::string strPieces = "pieces:  " + std::to_string(piecesTotal) + "/" + std::to_string(piecesStage);
        font.render(render, strPieces.c_str(), {xTextRight + 16.0f, yTextDown}, Render::WHITE);
    }
}
