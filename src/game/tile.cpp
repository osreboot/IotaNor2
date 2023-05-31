
#include <cstdlib>

#include "tile.h"

const float Tile::SIZE = 128.0f;
const float Tile::SIZE_S = SIZE - 16.0f;
const float Tile::SIZE_L = SIZE + 16.0f;

Tile::Tile() :
        illuminated(rand() % 2),
        visualSize(SIZE),
        quad(0, 0, SIZE, SIZE),
        quadLocked(0, 0, SIZE, SIZE) {}
