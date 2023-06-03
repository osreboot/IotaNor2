#pragma once

#include <deque>

#include "game/group.h"
#include "game/tile_floating.h"
#include "graphics/quad.h"
#include "graphics/texture.h"

class Game {

private:

public:
    static const int BOARD_DIM = 7;
    static const int GROUP_QUEUE_SIZE = 4;
    static const int STAGES = 8;

    static Coordf getWorld(Coordi tile);
    static Coordi getTile(Coordf world);

    static Coordf getOriginQueue(int index);
    static Coordf getOriginHold();

    Quad quadDebugCursor;

    Tile tiles[BOARD_DIM][BOARD_DIM];

    std::deque<Group*> groups;
    Group* groupHold;

    int stage;
    float timerInfect;

    Game();
    ~Game();

    void update(float delta);

    bool isInfectable(int x, int y) const;

    bool getStageGoal() const;
    float getStageInfectFreq() const;
    float getStageInfectTime() const;
    int getStageInfectBatch() const;

};
