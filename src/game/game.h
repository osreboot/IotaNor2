#pragma once

#include <deque>

#include "game/ui/stats.h"
#include "game/audio/audio.h"
#include "game/group.h"
#include "game/tile_floating.h"
#include "graphics/quad.h"
#include "graphics/texture.h"
#include "vec2f.h"
#include "vec2i.h"

// Class that manages the core game cycle
class Game {

private:

public:
    static const int BOARD_DIM = 7; // Board size in a single dimension
    static const int GROUP_QUEUE_SIZE = 4; // Number of groups stored to the left of the playing field
    static const int STAGES = 8; // Number of stages that the player has to clear to win

    static vec2f getWorld(vec2i tile); // Converts board ('tile') coordinate space to screen ('world') coordinates
    static vec2i getTile(vec2f world); // Converts screen ('world') coordinate space to board ('tile') coordinates

    static vec2f getOriginQueue(int index); // The world location for the piece queue (to the left of the board)
    static vec2f getOriginHold(); // The world location for held piece storage (to the right of the board)

    // Board tiles
    Tile tiles[BOARD_DIM][BOARD_DIM];

    std::deque<Group*> groups; // Upcoming pieces
    Group* groupHold; // Held piece

    bool frozen; // Is the game on the end/start screen (and not actively running)
    float timerFrozenGrace; // Used to prevent the player from starting a new game too fast

    int stage; // Current stage number
    float timerInfect; // Time (in seconds) until new pieces are selected for infection

    // Managers for SFX and statistics text
    Audio audio;
    Stats stats;

    Game();
    ~Game();

    // Advances game state by 'delta' seconds
    void update(float delta);

    // Only tiles that are the same color as the goal and adjacent to non-goal tiles can be infected
    bool isInfectable(int x, int y) const;

    // Returns if the goal is to illuminate all tiles (true) or not (false)
    bool getStageGoal() const;

    float getStageInfectFreq() const; // The frequency with which to infect a new batch of tiles (given the current stage)
    float getStageInfectTime() const; // The time that an infected tile takes to change (given the current stage)
    int getStageInfectBatch() const;  // The number of tiles infected at once (given the current stage)

};
