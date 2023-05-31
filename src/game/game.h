#pragma once

#include "../quad.h"
#include "../texture.h"
#include "tile.h"

class Game {

private:

public:
    static const int BOARD_DIM = 7;

    static std::pair<float, float> getWorld(int tileX, int tileY);
    static std::pair<int, int> getTile(float worldX, float worldY);

    Quad quadDebugCursor, quadCursor;

    Tile tiles[BOARD_DIM][BOARD_DIM];

    Game();
    ~Game();

    void update(float delta);

};
