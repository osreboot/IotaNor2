
#include <algorithm>

#include "game/group.h"

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

Group::Group() : tiles() {
    tiles[0] = new Tile();
    tiles[4] = new Tile();
    tiles[7] = new Tile();

    calculatePadding();
}

Group::~Group() {
    for (Tile* tile : tiles) delete tile;
}

void Group::rotateCW() {
    Tile* tempDiag = tiles[0];
    tiles[0] = tiles[6];
    tiles[6] = tiles[8];
    tiles[8] = tiles[2];
    tiles[2] = tempDiag;

    Tile* tempAdj = tiles[1];
    tiles[1] = tiles[3];
    tiles[3] = tiles[7];
    tiles[7] = tiles[5];
    tiles[5] = tempAdj;

    calculatePadding();
}

void Group::rotateCCW() {
    Tile* tempDiag = tiles[0];
    tiles[0] = tiles[2];
    tiles[2] = tiles[8];
    tiles[8] = tiles[6];
    tiles[6] = tempDiag;

    Tile* tempAdj = tiles[1];
    tiles[1] = tiles[5];
    tiles[5] = tiles[7];
    tiles[7] = tiles[3];
    tiles[3] = tempAdj;

    calculatePadding();
}
