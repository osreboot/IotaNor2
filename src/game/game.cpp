
#include <cmath>

#include "game/game.h"
#include "graphics/display.h"
#include "util.h"

Coordf Game::getWorld(int tileX, int tileY) {
    const std::pair<GLsizei, GLsizei>& windowSize = display::getSize();
    const float worldTileMinX = (static_cast<float>(windowSize.first) / 2.0f) - (static_cast<float>(BOARD_DIM) * Tile::SIZE / 2.0f);
    const float worldTileMinY = (static_cast<float>(windowSize.second) / 2.0f) - (static_cast<float>(BOARD_DIM) * Tile::SIZE / 2.0f);
    const float worldTileMaxX = (static_cast<float>(windowSize.first) / 2.0f) + (static_cast<float>(BOARD_DIM) * Tile::SIZE / 2.0f);
    const float worldTileMaxY = (static_cast<float>(windowSize.second) / 2.0f) + (static_cast<float>(BOARD_DIM) * Tile::SIZE / 2.0f);

    return {map(static_cast<float>(tileX), 0, BOARD_DIM, worldTileMinX, worldTileMaxX),
            map(static_cast<float>(tileY), 0, BOARD_DIM, worldTileMinY, worldTileMaxY)};
}

Coordi Game::getTile(float worldX, float worldY) {
    const std::pair<GLsizei, GLsizei>& windowSize = display::getSize();
    const float worldTileMinX = (static_cast<float>(windowSize.first) / 2.0f) - (static_cast<float>(BOARD_DIM) * Tile::SIZE / 2.0f);
    const float worldTileMinY = (static_cast<float>(windowSize.second) / 2.0f) - (static_cast<float>(BOARD_DIM) * Tile::SIZE / 2.0f);
    const float worldTileMaxX = (static_cast<float>(windowSize.first) / 2.0f) + (static_cast<float>(BOARD_DIM) * Tile::SIZE / 2.0f);
    const float worldTileMaxY = (static_cast<float>(windowSize.second) / 2.0f) + (static_cast<float>(BOARD_DIM) * Tile::SIZE / 2.0f);

    return {static_cast<int>(floor(map(worldX, worldTileMinX, worldTileMaxX, 0, BOARD_DIM))),
            static_cast<int>(floor(map(worldY, worldTileMinY, worldTileMaxY, 0, BOARD_DIM)))};
}

Game::Game() : quadDebugCursor(0.0f, 0.0f, 384.0f, 384.0f),
               quadCursor(0.0f, 0.0f, Tile::SIZE, Tile::SIZE){

}

Game::~Game() = default;

void Game::update(float delta) {
    std::pair<GLfloat, GLfloat> locationCursor = display::getCursor();
    quadDebugCursor.x = locationCursor.first - 192.0f;
    quadDebugCursor.y = locationCursor.second - 192.0f;

    std::pair<int, int> tileCursor = getTile(locationCursor.first, locationCursor.second);
    if (tileCursor.first >= 0 && tileCursor.first < BOARD_DIM &&
        tileCursor.second >= 0 && tileCursor.second < BOARD_DIM) {
        //tiles[tileCursor.first][tileCursor.second].visualSize = Tile::SIZE_S;
        std::pair<float, float> worldTileCursor = getWorld(tileCursor.first, tileCursor.second);
        quadCursor.x = worldTileCursor.first;
        quadCursor.y = worldTileCursor.second;
    }
}
