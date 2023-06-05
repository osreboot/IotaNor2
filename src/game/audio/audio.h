#pragma once

#include "miniaudio/miniaudio.h"
#include "game/audio/audio_queue.h"

class Audio {

private:
    ma_engine engine;

    AudioQueue* queuePiecePlace;
    AudioQueue* queuePieceRotate;
    AudioQueue* queuePieceHold;

    ma_sound soundStageAdvance;

    void applyRandomPitch(ma_sound& sound, float pitchMin, float pitchMax);

public:
    bool muted;

    Audio();
    ~Audio();

    void onPiecePlace();
    void onPieceInfect();
    void onPieceRotate();
    void onPieceHold();
    void onStageAdvance();

};
