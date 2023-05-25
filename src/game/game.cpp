
#include "game.h"
#include "../display.h"

Game::Game() :
        quadCursor(0.0f, 0.0f, 384.0f, 384.0f),
        textureTest("../res/test.png"),
        textureTile("../res/tile_ref_3c.png") {

    tiles.reserve(4);
    tiles.emplace_back(0, 0);
    tiles.emplace_back(1, 0);
    tiles.emplace_back(-1, 0);
    tiles.emplace_back(2, 0);
}

Game::~Game() {}

void Game::update(float delta) {
    pair<GLfloat, GLfloat> locationCursor = display::getCursor();
    quadCursor.x = locationCursor.first - 192.0f;
    quadCursor.y = locationCursor.second - 192.0f;
}
