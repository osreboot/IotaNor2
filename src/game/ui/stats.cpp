
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <cmath>

#include "game/game.h"
#include "game/ui/stats.h"
#include "graphics/display.h"

const float Stats::POS_X = 88.0f;
const float Stats::POS_Y = 48.0f;
const float Stats::FONT_OFFSET_Y = 6.0f;

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

void Stats::draw(const Render& render, const Game& game, Font& font) {
    if (game.frozen) {

        // Display the title cards
        std::string strTitle = "Iota Nor";
        vec2f locationStrTitle = {Game::getOriginQueue(0).x - (font.getSize(strTitle.c_str()).x / 2.0f),
                                   Game::getOriginQueue(0).y + (Tile::SIZE / 2.0f) + FONT_OFFSET_Y};
        font.draw(render, strTitle.c_str(), locationStrTitle, Render::BLACK);

        std::string strCredit = "by Calvin Weaver";
        vec2f locationStrCredit = {Game::getOriginHold().x - (font.getSize(strCredit.c_str()).x / 2.0f),
                                    Game::getOriginHold().y + (Tile::SIZE / 2.0f) + FONT_OFFSET_Y};
        font.draw(render, strCredit.c_str(), locationStrCredit, Render::BLACK);

        // Check if the player just finished a game
        if (fmod(render.timer, 1.0) <= 0.5f && timerTotal > 0.0f) {
            // Display the player's score
            std::stringstream stream;
            stream << std::fixed << std::setprecision(2) << timerTotal;
            std::string strFinish = stream.str() + "  seconds            " + std::to_string(piecesTotal) + "  pieces";
            vec2f locationStrFinish = {((float)display::getSize().x / 2.0f) - (font.getSize(strFinish.c_str()).x / 2.0f), POS_Y + FONT_OFFSET_Y};
            font.draw(render, strFinish.c_str(), locationStrFinish, Render::WHITE);
        }

        // Check if another game is ready to start
        if (game.timerFrozenGrace <= 0.0f) {
            // Display start prompt
            std::string strStart = "click to start";
            vec2f locationStrStart = {((float)display::getSize().x / 2.0f) - (font.getSize(strStart.c_str()).x / 2.0f),
                                       (float)display::getSize().y - POS_Y + FONT_OFFSET_Y};
            font.draw(render, strStart.c_str(), locationStrStart, Render::WHITE);
        }
    } else {
        // Calculate text offsets for stats messages
        float yTextUp = POS_Y + FONT_OFFSET_Y;
        float yTextDown = (float)display::getSize().y - POS_Y + FONT_OFFSET_Y;
        float xTextLeft = ((float)display::getSize().x / 2.0f) - ((float)Game::BOARD_DIM * Tile::SIZE / 2.0f) + POS_X;
        float xTextRight = ((float)display::getSize().x / 2.0f) + ((float)Game::BOARD_DIM * Tile::SIZE / 2.0f) - POS_X - 104.0f;

        std::stringstream stream; // Used to convert float timers to fixed-precision strings

        // Display total elapsed time
        stream << std::fixed << std::setprecision(2) << timerTotal;
        std::string strTimeTotal = "total:  " + stream.str();
        font.draw(render, strTimeTotal.c_str(), {xTextLeft, yTextUp}, Render::WHITE);

        // Display time on the current stage
        stream.str(""); // Re-use the stringstream
        stream.clear();
        stream << std::fixed << std::setprecision(2) << timerStage;
        std::string strTimeStage = "current:  " + stream.str();
        font.draw(render, strTimeStage.c_str(), {xTextRight, yTextUp}, Render::WHITE);

        // Display stage number
        std::string strStage = "stage:  " + std::to_string(game.stage + 1);
        font.draw(render, strStage.c_str(), {xTextLeft, yTextDown}, Render::WHITE);

        // Display total pieces placed and pieces placed this stage
        std::string strPieces = "pieces:  " + std::to_string(piecesTotal) + "/" + std::to_string(piecesStage);
        font.draw(render, strPieces.c_str(), {xTextRight + 16.0f, yTextDown}, Render::WHITE);
    }
}
