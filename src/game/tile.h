#pragma once

#include "graphics/quad.h"

// Represents a tile (aligned to the board), the tile's infection state, and the tile's graphical resources and
// animation information.
class Tile {

private:
    bool illuminated;

public:
    static const float SIZE;
    static const float INFECTION_DISABLED; // Assign this value to 'timerInfect' to clear the tile's infection status

    // Returns the world location of a specific tile, given the location of its group and its index within the group
    static Coordf getGroupLocation(const Coordf& locationGroup, const Coordi& indexGroup);

    // Values related to tile visual animation state
    float visTimerLastUpdate, visTimerShock;

    // Time until the tile is flipped due to being infected, or 'INFECTION_DISABLED'
    float timerInfect;

    Quad quad;

    Tile(Tile const&) = delete;
    Tile& operator=(Tile const&) = delete;

    Tile();

    void setIlluminated(bool illuminatedArg);
    bool isIlluminated() const { return illuminated; }
    void flipIlluminated() { setIlluminated(!illuminated); }

};
