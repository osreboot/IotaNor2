#pragma once

#include "../quad.h"

class Tile {

private:

public:
    static const float SIZE;

    int x, y;
    Quad quad;

    Tile(int x, int y);

};
