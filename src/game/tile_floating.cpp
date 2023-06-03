
#include <cmath>
#include <functional>

#include "game/tile_floating.h"
#include "graphics/display.h"
#include "util.h"

TileFloating::TileFloating() : Tile() {
    visLocation.first = -static_cast<float>(display::getSize().first) / 2.0f;
    visLocation.second = static_cast<float>(display::getSize().second) / 2.0f;
}

void TileFloating::update(float delta, const Coordf &locationGroup, const Coordi &indexGroup) {
    Coordf locationTarget = getGroupLocation(locationGroup, indexGroup);
    float distanceTarget = distance(visLocation.first, visLocation.second, locationTarget.first, locationTarget.second);
    float speed = std::clamp(powf(distanceTarget * 8.0f, 1.3f), 300.0f, 8000.0f);

    if (speed * delta >= distanceTarget) {
        visLocation = locationTarget;
    } else {
        float angleTarget = std::atan2(locationTarget.second - visLocation.second, locationTarget.first - visLocation.first);
        visLocation.first += speed * delta * cosf(angleTarget);
        visLocation.second += speed * delta * sinf(angleTarget);
    }
}

Coordf TileFloating::getVisLocation(const Coordf &locationGroup, const Coordi &indexGroup) {
    return visLocation;
}
