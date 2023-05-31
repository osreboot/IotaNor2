#pragma once

#include "game/tile.h"
#include "graphics/quad.h"
#include "graphics/texture.h"

class Game {

private:

public:
    static const int BOARD_DIM = 7;

    static Coordf getWorld(int tileX, int tileY);
    static Coordi getTile(float worldX, float worldY);

    Quad quadDebugCursor, quadCursor;

    Tile tiles[BOARD_DIM][BOARD_DIM];

    Game();
    ~Game();

    void update(float delta);

};
