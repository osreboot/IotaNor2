#pragma once

#include "../quad.h"

class Tile {

private:

public:
    static const float SIZE, SIZE_S, SIZE_L;

    bool illuminated;
    float visualSize;

    Quad quad, quadLocked;

    Tile(Tile const&) = delete;
    Tile& operator=(Tile const&) = delete;

    Tile();

};
