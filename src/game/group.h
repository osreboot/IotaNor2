#pragma once

#include "game/tile.h"
#include "game/tile_floating.h"
#include "vec2f.h"

// Represents a 'piece'; a set of connected tiles that can be moved freely (off-grid) and can be rotated at 90 degree
// intervals.
class Group {

private:
    // Number of clockwise 90 degree rotations that have been applied
    int rotation;

    // Updates padding values based on the group's rotation
    void calculatePadding();

public:
    static const int DIM = 3;

    TileFloating* tiles[DIM * DIM];

    // Off-grid world origin
    vec2f location;

    // Represents the current 'shape' of the group. Each value is the number of tiles protruding from the middle of the
    // grid in each direction.
    int padL, padU, padR, padD;

    Group();
    ~Group();

    // Applies 90 degree rotations
    void rotateCW();
    void rotateCCW();

    // Resets group to the default orientation
    void rotationReset();

};
