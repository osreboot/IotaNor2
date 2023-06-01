
#include <cmath>
#include <functional>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "game/game.h"
#include "graphics/display.h"
#include "util.h"

Coordf Game::getWorld(Coordi tile) {
    const Coordw& windowSize = display::getSize();
    const float worldTileMinX = (static_cast<float>(windowSize.first) / 2.0f) - (static_cast<float>(BOARD_DIM) * Tile::SIZE / 2.0f);
    const float worldTileMinY = (static_cast<float>(windowSize.second) / 2.0f) - (static_cast<float>(BOARD_DIM) * Tile::SIZE / 2.0f);
    const float worldTileMaxX = (static_cast<float>(windowSize.first) / 2.0f) + (static_cast<float>(BOARD_DIM) * Tile::SIZE / 2.0f);
    const float worldTileMaxY = (static_cast<float>(windowSize.second) / 2.0f) + (static_cast<float>(BOARD_DIM) * Tile::SIZE / 2.0f);

    return {map(static_cast<float>(tile.first), 0, BOARD_DIM, worldTileMinX, worldTileMaxX),
            map(static_cast<float>(tile.second), 0, BOARD_DIM, worldTileMinY, worldTileMaxY)};
}

Coordi Game::getTile(Coordf world) {
    const Coordw& windowSize = display::getSize();
    const float worldTileMinX = (static_cast<float>(windowSize.first) / 2.0f) - (static_cast<float>(BOARD_DIM) * Tile::SIZE / 2.0f);
    const float worldTileMinY = (static_cast<float>(windowSize.second) / 2.0f) - (static_cast<float>(BOARD_DIM) * Tile::SIZE / 2.0f);
    const float worldTileMaxX = (static_cast<float>(windowSize.first) / 2.0f) + (static_cast<float>(BOARD_DIM) * Tile::SIZE / 2.0f);
    const float worldTileMaxY = (static_cast<float>(windowSize.second) / 2.0f) + (static_cast<float>(BOARD_DIM) * Tile::SIZE / 2.0f);

    return {static_cast<int>(floor(map(world.first, worldTileMinX, worldTileMaxX, 0, BOARD_DIM))),
            static_cast<int>(floor(map(world.second, worldTileMinY, worldTileMaxY, 0, BOARD_DIM)))};
}

Game::Game() : quadDebugCursor(0.0f, 0.0f, 384.0f, 384.0f) {

    // Initialize board
    for (int x = 0; x < BOARD_DIM; x++) {
        for (int y = 0; y < BOARD_DIM; y++) {
            Coordf world = getWorld({x, y});
            tiles[x][y].quad.x = world.first;
            tiles[x][y].quad.y = world.second;
        }
    }

    // Initialize group queue
    for (int i = 0; i < GROUP_QUEUE_SIZE; i++) groups.emplace_back();

}

void Game::update(float delta) {
    Coordf locationCursor = display::getCursor();
    quadDebugCursor.x = locationCursor.first - 192.0f;
    quadDebugCursor.y = locationCursor.second - 192.0f;

    Group& group = groups.front();

    // Handle group rotations
    if (display::hasEventKeyPress(GLFW_KEY_A)) {
        group.rotateCCW();
    } else if (display::hasEventKeyPress(GLFW_KEY_D)) {
        group.rotateCW();
    }

    // Clamp group position
    Coordi tileCursor = getTile(locationCursor);
    tileCursor.first = std::clamp(static_cast<int>(tileCursor.first), group.padL, BOARD_DIM - 1 - group.padR);
    tileCursor.second = std::clamp(static_cast<int>(tileCursor.second), group.padU, BOARD_DIM - 1 - group.padD);
    group.location = getWorld(tileCursor);
    group.location.first += Tile::SIZE / 2.0f;
    group.location.second += Tile::SIZE / 2.0f;

    // Handle group placing
    if (display::hasEventMouseRelease()) {
        for (int x = 0; x < Group::DIM; x++) {
            for (int y = 0; y < Group::DIM; y++) {
                if (group.tiles[y * Group::DIM + x]) {
                    Tile& tile = tiles[tileCursor.first + x - (Group::DIM / 2)][tileCursor.second + y - (Group::DIM / 2)];
                    tile.illuminated = !tile.illuminated;
                }
            }
        }

        groups.pop_front();
        groups.emplace_back();
    }
}
