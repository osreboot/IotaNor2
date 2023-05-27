
#include "game.h"
#include "../display.h"

const int Game::BOARD_SIZE = 3;

Game::Game() :
        quadCursor(0.0f, 0.0f, 384.0f, 384.0f),
        textureTest("../res/test.png"),
        textureTile("../res/tile_ref_3c.png") {

    int boardDim = BOARD_SIZE / 2;

    tiles.reserve(BOARD_SIZE * BOARD_SIZE);
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
