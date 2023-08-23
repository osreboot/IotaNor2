#pragma once

#include "game/tile.h"

// Represents a tile that is not aligned to the board (e.g. part of a free-moving group / piece)
class TileFloating : public Tile {

private:

public:
    // The current world location of the tile's sprite
    vec2f visLocation;

    TileFloating();

    // Advances tile state by 'delta' seconds
    void update(float delta, const vec2f &locationGroup, const vec2i &indexGroup);

};
