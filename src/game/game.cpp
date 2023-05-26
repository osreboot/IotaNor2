
#include "game.h"
#include "../display.h"

Game::Game() :
        quadCursor(0.0f, 0.0f, 384.0f, 384.0f),
        textureTest("../res/test.png"),
        textureTile("../res/tile_ref_3c.png") {

    tiles.reserve(25);
    for(int x = -2; x <= 2; x++) {
        for(int y = -2; y <= 2; y++) {
            tiles.emplace_back(x, y);
        }
    }
}

Game::~Game() {}

void Game::update(float delta) {
    pair<GLfloat, GLfloat> locationCursor = display::getCursor();
    quadCursor.x = locationCursor.first - 192.0f;
    quadCursor.y = locationCursor.second - 192.0f;
}
