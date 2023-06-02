#pragma once

#include "game/tile.h"

class Group {

private:
    int rotation;

    void calculatePadding();

public:
    static const int DIM = 3;

    Tile* tiles[DIM * DIM];
    Coordf location;

    int padL, padU, padR, padD;

    Group();
    ~Group();

    void rotateCW();
    void rotateCCW();

    void rotationReset();

};
