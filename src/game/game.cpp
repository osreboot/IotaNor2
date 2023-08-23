
#include <cfloat>
#include <cmath>
#include <functional>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "game/game.h"
#include "graphics/display.h"
#include "util.h"

vec2f Game::getWorld(vec2i tile) {
    // Calculate board boundaries (in world coordinates)
    const vec2i& windowSize = display::getSize();
    const float worldTileMinX = ((float)windowSize.x / 2.0f) - ((float)BOARD_DIM * Tile::SIZE / 2.0f);
    const float worldTileMinY = ((float)windowSize.y / 2.0f) - ((float)BOARD_DIM * Tile::SIZE / 2.0f);
    const float worldTileMaxX = ((float)windowSize.x / 2.0f) + ((float)BOARD_DIM * Tile::SIZE / 2.0f);
    const float worldTileMaxY = ((float)windowSize.y / 2.0f) + ((float)BOARD_DIM * Tile::SIZE / 2.0f);

    // Convert tile coordinates to world coordinate space
    return {map((float)tile.x, 0, BOARD_DIM, worldTileMinX, worldTileMaxX),
            map((float)tile.y, 0, BOARD_DIM, worldTileMinY, worldTileMaxY)};
}

vec2i Game::getTile(vec2f world) {
    // Calculate board boundaries (in world coordinates)
    const vec2i& windowSize = display::getSize();
    const float worldTileMinX = ((float)windowSize.x / 2.0f) - ((float)BOARD_DIM * Tile::SIZE / 2.0f);
    const float worldTileMinY = ((float)windowSize.y / 2.0f) - ((float)BOARD_DIM * Tile::SIZE / 2.0f);
    const float worldTileMaxX = ((float)windowSize.x / 2.0f) + ((float)BOARD_DIM * Tile::SIZE / 2.0f);
    const float worldTileMaxY = ((float)windowSize.y / 2.0f) + ((float)BOARD_DIM * Tile::SIZE / 2.0f);

    // Convert world coordinates to tile coordinate space
    return {(int)std::floor(map((float)world.x, worldTileMinX, worldTileMaxX, 0, BOARD_DIM)),
            (int)std::floor(map((float)world.y, worldTileMinY, worldTileMaxY, 0, BOARD_DIM))};
}

vec2f Game::getOriginQueue(int index) {
    return getWorld({(-4 * (index + 1)) + 2, BOARD_DIM / 2});
}

vec2f Game::getOriginHold() {
    return getWorld({BOARD_DIM + 2, BOARD_DIM / 2});
}

Game::Game() : frozen(true),
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
    // Handle audio muting
    if (display::hasEventKeyPress(GLFW_KEY_M)) audio.muted = !audio.muted;

    if (frozen) { // Game is on the end/start screen (and isn't actively running)

        // Detect game start mouse click
        stepTowards(timerFrozenGrace, delta, 0.0f);
        if (timerFrozenGrace <= 0.0f && display::hasEventMouseRelease()) {
            frozen = false;
            stats.onGameStart();
        }

    } else { // Game is running

        // Fill the group queue
        if (groups.empty()) {
            for (int i = 0; i < GROUP_QUEUE_SIZE; i++) groups.push_back(new Group());
        }
        Group *group = groups.front();

        // Handle group rotations
        if (display::hasEventKeyPress(GLFW_KEY_A)) {
            group->rotateCCW();
            audio.onPieceRotate();
        } else if (display::hasEventKeyPress(GLFW_KEY_D)) {
            group->rotateCW();
            audio.onPieceRotate();
        }

        // Clamp group position to board
        vec2i tileCursor = getTile(display::getCursor());
        tileCursor.x = std::clamp(tileCursor.x, group->padL, BOARD_DIM - 1 - group->padR);
        tileCursor.y = std::clamp(tileCursor.y, group->padU, BOARD_DIM - 1 - group->padD);
        group->location = getWorld(tileCursor);
        group->location = group->location + (Tile::SIZE / 2.0f);

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

            audio.onPieceHold();
        } else if (display::hasEventMousePress()) { // Handle group placing
            for (int x = 0; x < Group::DIM; x++) {
                for (int y = 0; y < Group::DIM; y++) {
                    if (group->tiles[y * Group::DIM + x]) {
                        Tile &tile = tiles[tileCursor.x + x - (Group::DIM / 2)][tileCursor.y + y - (Group::DIM / 2)];
                        tile.flipIlluminated();

                        // // CHEAT: always-correct piece placement
                         tile.setIlluminated(getStageGoal());
                    }
                }
            }

            delete group;

            groups.pop_front();
            groups.push_back(new Group());

            audio.onPiecePlace();
            stats.onPiecePlace();
        }

        // Handle board infection spreading
        stepTowards(timerInfect, delta, 0.0f);
        if (timerInfect <= 0.0f) { // It's time to infect a new batch of tiles
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
        bool shouldPlayInfectionAudio = false;
        for (int x = 0; x < BOARD_DIM; x++) {
            for (int y = 0; y < BOARD_DIM; y++) {
                Tile &tile = tiles[x][y];
                if (tile.timerInfect != Tile::INFECTION_DISABLED) {
                    stepTowards(tile.timerInfect, delta, 0.0f);
                    if (tile.timerInfect == 0.0f) {
                        tile.timerInfect = Tile::INFECTION_DISABLED;
                        tile.flipIlluminated();
                        shouldPlayInfectionAudio = true;
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

            audio.onStageAdvance();
            stats.onStageAdvance();

            // Locate the most recently updated tile (for stage clear ripple animation)
            vec2i tileLastUpdate = {0, 0};
            float deltaLastUpdate = FLT_MAX;
            for (int x = 0; x < BOARD_DIM; x++) {
                for (int y = 0; y < BOARD_DIM; y++) {
                    if (tiles[x][y].visTimerLastUpdate < deltaLastUpdate) {
                        deltaLastUpdate = tiles[x][y].visTimerLastUpdate;
                        tileLastUpdate = {x, y};
                    }
                }
            }

            // Display ripple animation and clear infections
            for (int x = 0; x < BOARD_DIM; x++) {
                for (int y = 0; y < BOARD_DIM; y++) {
                    Tile &tile = tiles[x][y];
                    tile.timerInfect = Tile::INFECTION_DISABLED;
                    tile.visTimerLastUpdate = 0.0f;
                    tile.visTimerShock = 1.0f + distance(x, y, tileLastUpdate.x, tileLastUpdate.y) / 3.0f;
                }
            }
        } else if (shouldPlayInfectionAudio) audio.onPieceInfect();

        // Update group origins for the piece queue
        for (int i = 1; i < GROUP_QUEUE_SIZE; i++) {
            groups[i]->location = getOriginQueue(i - 1);
            groups[i]->location.y += Tile::SIZE / 2.0f;
        }

        // Update group origin of the held piece
        if (groupHold) {
            groupHold->location = getOriginHold();
            groupHold->location.y += Tile::SIZE / 2.0f;
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
