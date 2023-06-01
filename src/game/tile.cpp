
#include <cstdlib>

#include "tile.h"

const float Tile::SIZE = 128.0f;

Tile::Tile() :
        illuminated(rand() % 2),
        quad(0, 0, SIZE, SIZE) {}
