
#include <cfloat>
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

Coordf Game::getOriginQueue(int index) {
    return getWorld({(-4 * (index + 1)) + 2, BOARD_DIM / 2});
}

Coordf Game::getOriginHold() {
    return getWorld({BOARD_DIM + 2, BOARD_DIM / 2});
}

Game::Game() : quadDebugCursor(0.0f, 0.0f, 384.0f, 384.0f),
               frozen(true),
               timerFrozenGrace(0.0f),
               stage(0),
               timerInfect(getStageInfectFreq()) {

    groupHold = nullptr;
}

Game::~Game() {
    for (Group* group : groups) delete group;
    delete groupHold;
}

void Game::update(float delta) {
    Coordf locationCursor = display::getCursor();
    quadDebugCursor.x = locationCursor.first - 192.0f;
    quadDebugCursor.y = locationCursor.second - 192.0f;

    if (frozen) {

        stepTowards(timerFrozenGrace, delta, 0.0f);
        if (timerFrozenGrace <= 0.0f && display::hasEventMouseRelease()) {
            frozen = false;
            stats.onGameStart();
        }

    } else {

        if (groups.empty()) {
            for (int i = 0; i < GROUP_QUEUE_SIZE; i++) groups.push_back(new Group());
        }
        Group *group = groups.front();

        // Handle group rotations
        if (display::hasEventKeyPress(GLFW_KEY_A)) {
            group->rotateCCW();
        } else if (display::hasEventKeyPress(GLFW_KEY_D)) {
            group->rotateCW();
        }

        // Clamp group position to board
        Coordi tileCursor = getTile(locationCursor);
        tileCursor.first = std::clamp(static_cast<int>(tileCursor.first), group->padL, BOARD_DIM - 1 - group->padR);
        tileCursor.second = std::clamp(static_cast<int>(tileCursor.second), group->padU, BOARD_DIM - 1 - group->padD);
        group->location = getWorld(tileCursor);
        group->location.first += Tile::SIZE / 2.0f;
        group->location.second += Tile::SIZE / 2.0f;

        // Handle group holding
        if (display::hasEventKeyPress(GLFW_KEY_SPACE)) {
            Group *groupTemp = groupHold;
            groupHold = groups.front();
            groupHold->rotationReset();
            if (groupTemp) {
                groups.front() = groupTemp;
            } else {
                groups.pop_front();
                groups.push_back(new Group());
            }
        } else if (display::hasEventMouseRelease()) { // Handle group placing
            for (int x = 0; x < Group::DIM; x++) {
                for (int y = 0; y < Group::DIM; y++) {
                    if (group->tiles[y * Group::DIM + x]) {
                        Tile &tile = tiles[tileCursor.first + x - (Group::DIM / 2)][tileCursor.second + y -
                                                                                    (Group::DIM / 2)];
                        tile.flipIlluminated();
                        //tile.setIlluminated(getStageGoal());
                    }
                }
            }

            delete group;

            groups.pop_front();
            groups.push_back(new Group());

            stats.onPiecePlace();
        }

        // Handle board infection spreading
        stepTowards(timerInfect, delta, 0.0f);
        if (timerInfect <= 0.0f) {
            timerInfect = getStageInfectFreq();

            // Count how many tiles are eligible for infection
            int numInfectable = 0;
            for (int x = 0; x < BOARD_DIM; x++) {
                for (int y = 0; y < BOARD_DIM; y++) {
                    if (isInfectable(x, y)) numInfectable++;
                }
            }

            // Infect eligible tiles
            int numToInfect = getStageInfectBatch();
            while (numInfectable > 0 && numToInfect > 0) {
                int indexInfected = rand() % numInfectable;
                int indexReal = -1;
                for (int i = 0; i < numInfectable; i++) {
                    do indexReal++;
                    while (!isInfectable(indexReal % BOARD_DIM, indexReal / BOARD_DIM));
                    if (indexInfected == i) {
                        tiles[indexReal % BOARD_DIM][indexReal / BOARD_DIM].timerInfect = getStageInfectTime();
                        break;
                    }
                }

                numInfectable--;
                numToInfect--;
            }
        }

        // Handle individual tile infections
        for (int x = 0; x < BOARD_DIM; x++) {
            for (int y = 0; y < BOARD_DIM; y++) {
                Tile &tile = tiles[x][y];
                if (tile.timerInfect != Tile::INFECTION_DISABLED) {
                    stepTowards(tile.timerInfect, delta, 0.0f);
                    if (tile.timerInfect == 0.0f) {
                        tile.timerInfect = Tile::INFECTION_DISABLED;
                        tile.flipIlluminated();
                    }
                }
            }
        }

        // Handle stage progression
        bool stageComplete = true;
        for (int x = 0; x < BOARD_DIM; x++) {
            for (int y = 0; y < BOARD_DIM; y++) {
                if (tiles[x][y].isIlluminated() != getStageGoal()) {
                    stageComplete = false;
                    break;
                }
            }
            if (!stageComplete) break;
        }
        if (stageComplete) { // Advance stage
            stage++;
            if (stage == STAGES) { // Game is finished!
                stage = 0;

                for (Group* g : groups) delete g;
                groups.clear();

                delete groupHold;
                groupHold = nullptr;

                frozen = true;
                timerFrozenGrace = 5.0f;
            }

            timerInfect = getStageInfectFreq();

            stats.onStageAdvance();

            // Locate the most recently updated tile (for stage clear ripple animation)
            Coordi tileLastUpdate = {0, 0};
            float deltaLastUpdate = FLT_MAX;
            for (int x = 0; x < BOARD_DIM; x++) {
                for (int y = 0; y < BOARD_DIM; y++) {
                    if (tiles[x][y].visTimerLastUpdate < deltaLastUpdate) {
                        deltaLastUpdate = tiles[x][y].visTimerLastUpdate;
                        tileLastUpdate.first = x;
                        tileLastUpdate.second = y;
                    }
                }
            }

            // Display ripple animation and clear infections
            for (int x = 0; x < BOARD_DIM; x++) {
                for (int y = 0; y < BOARD_DIM; y++) {
                    Tile &tile = tiles[x][y];
                    tile.timerInfect = Tile::INFECTION_DISABLED;
                    tile.visTimerLastUpdate = 0.0f;
                    tile.visTimerShock = 1.0f + distance(x, y, tileLastUpdate.first, tileLastUpdate.second) / 3.0f;
                }
            }
        }

        for (int i = 1; i < GROUP_QUEUE_SIZE; i++) {
            groups[i]->location = getOriginQueue(i - 1);
            groups[i]->location.second += Tile::SIZE / 2.0f;
        }

        if (groupHold) {
            groupHold->location = getOriginHold();
            groupHold->location.second += Tile::SIZE / 2.0f;
        }

    }

    stats.update(delta, *this);
}

bool Game::isInfectable(int x, int y) const {
    bool stageGoal = getStageGoal();
    if (tiles[x][y].isIlluminated() != stageGoal || tiles[x][y].timerInfect != Tile::INFECTION_DISABLED) return false;
    if (x - 1 >= 0 && tiles[x - 1][y].isIlluminated() != stageGoal) return true;
    if (x + 1 <= BOARD_DIM - 1 && tiles[x + 1][y].isIlluminated() != stageGoal) return true;
    if (y - 1 >= 0 && tiles[x][y - 1].isIlluminated() != stageGoal) return true;
    if (y + 1 <= BOARD_DIM - 1 && tiles[x][y + 1].isIlluminated() != stageGoal) return true;
    return false;
}

bool Game::getStageGoal() const {
    return !(stage % 2);
}

float Game::getStageInfectFreq() const {
    return map(static_cast<float>(stage), 1.0f, STAGES, 16.0f, 4.0f);
}

float Game::getStageInfectTime() const {
    static const float stageTimes[] = {
            FLT_MAX,
            10.0f,
            8.0f,
            8.0f,
            6.0f,
            4.0f,
            3.5f,
            3.0f,
    };
    if (stage >= sizeof(stageTimes) / sizeof(float))
        return stageTimes[(sizeof(stageTimes) / sizeof(float)) - 1];
    return stageTimes[stage];
}

int Game::getStageInfectBatch() const {
    if (stage == 0) return 0;
    return stage < 3 ? 1 : 2;
}
