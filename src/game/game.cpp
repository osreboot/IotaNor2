
#include "game.h"
#include "../display.h"

const int Game::BOARD_DIM = 3;

Game::Game() : quadCursor(0.0f, 0.0f, 384.0f, 384.0f) {

    const int boardDim = BOARD_DIM / 2;

    for(int x = -boardDim; x <= boardDim; x++) {
        for(int y = -boardDim; y <= boardDim; y++) {
            tiles.emplace_back(x, y);
        }
    }
}

Game::~Game() = default;

void Game::update(float delta) {
    pair<GLfloat, GLfloat> locationCursor = display::getCursor();
    quadCursor.x = locationCursor.first - 192.0f;
    quadCursor.y = locationCursor.second - 192.0f;
}
