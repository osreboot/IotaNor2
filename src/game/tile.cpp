
#include <cstdlib>
#include "tile.h"
#include "../display.h"

const float Tile::SIZE = 128.0f;

Tile::Tile(int x, int y) : x(x), y(y), illum(rand() % 2),
        quad((static_cast<float>(x) * SIZE) - (SIZE / 2.0f) + (static_cast<float>(display::getSize().first) / 2.0f),
             (static_cast<float>(y) * SIZE) - (SIZE / 2.0f) + (static_cast<float>(display::getSize().second) / 2.0f),
             SIZE, SIZE) {}
