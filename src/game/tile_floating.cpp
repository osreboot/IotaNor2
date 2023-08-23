
#include <cmath>
#include <functional>

#include "game/tile_floating.h"
#include "graphics/display.h"
#include "util.h"

TileFloating::TileFloating() : Tile() {
    // Start the tile off-screen to the left
    visLocation.x = -(float)display::getSize().x / 2.0f;
    visLocation.y = (float)display::getSize().y / 2.0f;
}

void TileFloating::update(float delta, const vec2f& locationGroup, const vec2i& indexGroup) {
    // Calculate the tile's target position and distance
    vec2f locationTarget = getGroupLocation(locationGroup, indexGroup);
    float distanceTarget = distance(visLocation.x, visLocation.y, locationTarget.x, locationTarget.y);

    // Ease the tile towards its target
    float speed = std::clamp(powf(distanceTarget * 8.0f, 1.3f), 300.0f, 8000.0f);

    // Lock to target if close enough, otherwise step towards
    if (speed * delta >= distanceTarget) {
        visLocation = locationTarget;
    } else {
        float angleTarget = std::atan2(locationTarget.y - visLocation.y, locationTarget.x - visLocation.x);
        visLocation.x += speed * delta * cosf(angleTarget);
        visLocation.y += speed * delta * sinf(angleTarget);
    }
}
