#pragma once

#include <vector>

#include "../quad.h"
#include "../texture.h"
#include "tile.h"

class Game {

private:

public:
    static const int BOARD_SIZE;

    Quad quadCursor;
    Texture textureTest;
    Texture textureTile;

    vector<Tile> tiles;

    Game();
    ~Game();

    void update(float delta);

};
