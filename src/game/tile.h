#pragma once

#include "graphics/quad.h"

class Tile {

private:

public:
    static const float SIZE;

    bool illuminated;

    Quad quad;

    Tile(Tile const&) = delete;
    Tile& operator=(Tile const&) = delete;

    Tile();

};
