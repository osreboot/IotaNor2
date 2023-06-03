
#include "tile.h"
#include "group.h"

const float Tile::SIZE = 128.0f;
const float Tile::INFECTION_DISABLED = -1.0f;

Coordf Tile::getGroupLocation(const Coordf &locationGroup, const Coordi &indexGroup) {
    const float offsetGroup = static_cast<float>(Group::DIM) * SIZE / 2.0f;
    return {locationGroup.first - offsetGroup + static_cast<float>(indexGroup.first) * SIZE,
            locationGroup.second - offsetGroup + static_cast<float>(indexGroup.second) * SIZE};
}

Tile::Tile() :
        illuminated(false),
        visTimerLastUpdate(0.0f),
        visTimerShock(0.0f),
        timerInfect(INFECTION_DISABLED),
        quad(0, 0, SIZE, SIZE) {}

void Tile::setIlluminated(bool illuminatedArg) {
    illuminated = illuminatedArg;
    visTimerLastUpdate = 0.0f;
    visTimerShock = 1.0f;
}

Coordf Tile::getVisLocation(const Coordf& locationGroup, const Coordi& indexGroup) {
    return getGroupLocation(locationGroup, indexGroup);
}
