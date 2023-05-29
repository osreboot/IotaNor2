#pragma once

#include <deque>

#include "../quad.h"
#include "../texture.h"
#include "tile.h"

class Game {

private:

public:
    static const int BOARD_DIM;

    Quad quadCursor;

    deque<Tile> tiles;

    Game();
    ~Game();

    void update(float delta);

};
