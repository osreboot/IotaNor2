
#include <algorithm>

#include "game/group.h"
#include "game/tile_floating.h"

static const Coordi SHAPES[] = {
        {0, 5}, // j
        {2, 3},
        {0, 8}, // /
        {0, 8},
        {1, 7}, // |
        {1, 7},
        {0, 2}, // v
        {0, 2},
        {1, 5}, // L
        {1, 5},

        /*
        {0, 6}, // >
        {1, 6}, // j
        {2, 6}, // /
        {0, 7}, // j
        {1, 7}, // |
        {3, 7}, // r
         */
};

void Group::calculatePadding() {
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
    padL = std::clamp((DIM / 2) - xMin, 0, (DIM / 2));
    padU = std::clamp((DIM / 2) - yMin, 0, (DIM / 2));
    padR = std::clamp(xMax - (DIM / 2), 0, (DIM / 2));
    padD = std::clamp(yMax - (DIM / 2), 0, (DIM / 2));
}

Group::Group() : rotation(0), tiles() {
    // Create random shape
    Coordi tilesShape = SHAPES[rand() % (sizeof(SHAPES) / sizeof(Coordi))];
    tiles[4] = new TileFloating();
    tiles[tilesShape.first] = new TileFloating();
    tiles[tilesShape.second] = new TileFloating();

    calculatePadding();
}

Group::~Group() {
    for (Tile* tile : tiles) delete tile;
}

void Group::rotateCW() {
    rotation = (rotation + 1) % 4;

    TileFloating* tempDiag = tiles[0];
    tiles[0] = tiles[6];
    tiles[6] = tiles[8];
    tiles[8] = tiles[2];
    tiles[2] = tempDiag;

    TileFloating* tempAdj = tiles[1];
    tiles[1] = tiles[3];
    tiles[3] = tiles[7];
    tiles[7] = tiles[5];
    tiles[5] = tempAdj;

    calculatePadding();
}

void Group::rotateCCW() {
    rotation = (rotation + 3) % 4;

    TileFloating* tempDiag = tiles[0];
    tiles[0] = tiles[2];
    tiles[2] = tiles[8];
    tiles[8] = tiles[6];
    tiles[6] = tempDiag;

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
