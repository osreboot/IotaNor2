
#include <algorithm>

#include "game/group.h"
#include "game/tile_floating.h"
#include "vec2i.h"

// Basic shapes, values represent group tile indices (plus the middle piece (4) which is always present). Array is
// indexed randomly, so some shapes appear twice to influence the selection probability
static const vec2i SHAPES[] = {
        {0, 5}, // j
        {2, 3},
        {0, 8}, // /
        {0, 8},
        {1, 7}, // |
        {1, 7},
        {6, 8}, // v
        {6, 8},
        {1, 5}, // L
        {1, 5},
};

// Examines the 'tiles' array and updates padding values based on the current layout
void Group::calculatePadding() {
    // Find the bounds where tiles are present
    int xMin = DIM;
    int yMin = DIM;
    int xMax = 0;
    int yMax = 0;
    for (int x = 0; x < DIM; x++) {
        for (int y = 0; y < DIM; y++) {
            if (tiles[y * DIM + x]) {
                xMin = std::min(xMin, x);
                yMin = std::min(yMin, y);
                xMax = std::max(xMax, x);
                yMax = std::max(yMax, y);
            }
        }
    }

    // Update padding based on bounds, with origin offset applied
    padL = std::clamp((DIM / 2) - xMin, 0, (DIM / 2));
    padU = std::clamp((DIM / 2) - yMin, 0, (DIM / 2));
    padR = std::clamp(xMax - (DIM / 2), 0, (DIM / 2));
    padD = std::clamp(yMax - (DIM / 2), 0, (DIM / 2));
}

Group::Group() : rotation(0), tiles() {
    // Create random shape
    vec2i tilesShape = SHAPES[rand() % (sizeof(SHAPES) / sizeof(vec2i))];
    tiles[4] = new TileFloating();
    tiles[tilesShape.x] = new TileFloating();
    tiles[tilesShape.y] = new TileFloating();

    calculatePadding();
}

Group::~Group() {
    for (Tile* tile : tiles) delete tile;
}

void Group::rotateCW() {
    rotation = (rotation + 1) % 4;

    // Swap diagonal tiles
    TileFloating* tempDiag = tiles[0];
    tiles[0] = tiles[6];
    tiles[6] = tiles[8];
    tiles[8] = tiles[2];
    tiles[2] = tempDiag;

    // Swap adjacent tiles
    TileFloating* tempAdj = tiles[1];
    tiles[1] = tiles[3];
    tiles[3] = tiles[7];
    tiles[7] = tiles[5];
    tiles[5] = tempAdj;

    calculatePadding();
}

void Group::rotateCCW() {
    rotation = (rotation + 3) % 4;

    // Swap diagonal tiles
    TileFloating* tempDiag = tiles[0];
    tiles[0] = tiles[2];
    tiles[2] = tiles[8];
    tiles[8] = tiles[6];
    tiles[6] = tempDiag;

    // Swap adjacent tiles
    TileFloating* tempAdj = tiles[1];
    tiles[1] = tiles[5];
    tiles[5] = tiles[7];
    tiles[7] = tiles[3];
    tiles[3] = tempAdj;

    calculatePadding();
}

void Group::rotationReset() {
    while (rotation > 0) rotateCCW();
}
