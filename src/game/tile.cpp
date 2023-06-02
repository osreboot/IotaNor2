
#include "tile.h"

const float Tile::SIZE = 128.0f;
const float Tile::INFECTION_DISABLED = -1.0f;

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
