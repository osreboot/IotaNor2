#pragma once

#include "miniaudio/miniaudio.h"
#include "game/audio/audio_queue.h"

// Handles the loading and playing of game sound effects
class Audio {

private:
    // Applies a random pitch to the sound reference
    static void applyRandomPitch(ma_sound& sound, float pitchMin, float pitchMax);

    ma_engine engine;

    // These sounds can be played faster than their total runtime; so a queue of ma_sound instances is needed to ensure
    // that we can keep up with the number of requests
    AudioQueue* queuePiecePlace;
    AudioQueue* queuePieceRotate;
    AudioQueue* queuePieceHold;

    // This sound isn't played frequently and doesn't need multiple instances
    ma_sound soundStageAdvance;

public:
    // If the player has muted all sounds
    bool muted;

    // All sounds are loaded on object construction and released on destruction
    Audio();
    ~Audio();

    // These functions should be called when game events happen to request that sounds be played!
    void onPiecePlace();
    void onPieceInfect();
    void onPieceRotate();
    void onPieceHold();
    void onStageAdvance();

};
