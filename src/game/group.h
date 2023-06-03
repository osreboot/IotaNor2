#pragma once

#include "game/tile.h"
#include "game/tile_floating.h"

class Group {

private:
    int rotation;

    void calculatePadding();

public:
    static const int DIM = 3;

    TileFloating* tiles[DIM * DIM];
    Coordf location;

    int padL, padU, padR, padD;

    Group();
    ~Group();

    void rotateCW();
    void rotateCCW();

    void rotationReset();

};
