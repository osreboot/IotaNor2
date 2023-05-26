#pragma once

#include <vector>

#include "../quad.h"
#include "../texture.h"
#include "tile.h"

class Game {

private:

public:
    Quad quadCursor;
    Texture textureTest;
    Texture textureTile;

    vector<Tile> tiles;

    Game();
    ~Game();

    void update(float delta);

};
