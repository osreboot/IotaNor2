
#include "tile.h"
#include "group.h"

const float Tile::SIZE = 128.0f;
const float Tile::INFECTION_DISABLED = -1.0f;

vec2f Tile::getGroupLocation(const vec2f& locationGroup, const vec2i& indexGroup) {
    const float offsetGroup = (float)Group::DIM * SIZE / 2.0f;
    return locationGroup - offsetGroup + (vec2f)indexGroup * SIZE;
    //return {locationGroup.x - offsetGroup + (float)indexGroup.x * SIZE,
    //        locationGroup.y - offsetGroup + (float)indexGroup.y * SIZE};
}

Tile::Tile() :
        illuminated(false),
        visTimerLastUpdate(0.0f),
        visTimerShock(0.0f),
        timerInfect(INFECTION_DISABLED),
        quad(0, 0, SIZE, SIZE) {}

void Tile::setIlluminated(bool illuminatedArg) {
    illuminated = illuminatedArg;

    // Update visual animation state due to tile change
    visTimerLastUpdate = 0.0f;
    visTimerShock = 1.0f;
}
