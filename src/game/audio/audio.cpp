
#include <cstdlib>

#include "game/audio/audio.h"
#include "util.h"

void Audio::applyRandomPitch(ma_sound& sound, float pitchMin, float pitchMax) {
    ma_sound_set_pitch(&sound, map(static_cast<float>(rand() % 1000), 0.0f, 1000.0f, pitchMin, pitchMax));
}

Audio::Audio() : muted(false) {
    // Initialize miniaudio engine
    ma_engine_init(nullptr, &engine);

    // Initialize our sound queues
    queuePiecePlace = new AudioQueue(engine, 4, "res/audio/piece_place.wav");
    queuePieceRotate = new AudioQueue(engine, 4, "res/audio/piece_rotate.wav");
    queuePieceHold = new AudioQueue(engine, 4, "res/audio/piece_hold.wav");

    // Load the sound that doesn't need a queue
    ma_sound_init_from_file(&engine, "res/audio/stage_advance.wav", 0, nullptr, nullptr, &soundStageAdvance);
    ma_sound_set_volume(&soundStageAdvance, 0.3f);
}

Audio::~Audio() {
    delete queuePiecePlace;
    delete queuePieceRotate;
    delete queuePieceHold;

    ma_engine_uninit(&engine);
}

void Audio::onPiecePlace() {
    if (!muted) {
        ma_sound_set_volume(&queuePiecePlace->getCurrent(), 2.5f);
        applyRandomPitch(queuePiecePlace->getCurrent(), 0.9f, 1.4f);
        ma_sound_start(&queuePiecePlace->getCurrent());
        queuePiecePlace->advance(); // Cycle queue so it's ready for the next sound event
    }
}

void Audio::onPieceInfect() {
    if (!muted) {
        ma_sound_set_volume(&queuePiecePlace->getCurrent(), 2.5f);
        applyRandomPitch(queuePiecePlace->getCurrent(), 0.6f, 0.8f);
        ma_sound_start(&queuePiecePlace->getCurrent());
        queuePiecePlace->advance(); // Cycle queue so it's ready for the next sound event
    }
}

void Audio::onPieceRotate() {
    if (!muted) {
        ma_sound_set_volume(&queuePieceRotate->getCurrent(), 0.25f);
        applyRandomPitch(queuePieceRotate->getCurrent(), 0.9f, 1.1f);
        ma_sound_start(&queuePieceRotate->getCurrent());
        queuePieceRotate->advance(); // Cycle queue so it's ready for the next sound event
    }
}

void Audio::onPieceHold() {
    if (!muted) {
        ma_sound_set_volume(&queuePieceHold->getCurrent(), 0.3f);
        ma_sound_start(&queuePieceHold->getCurrent());
        queuePieceHold->advance(); // Cycle queue so it's ready for the next sound event
    }
}

void Audio::onStageAdvance() {
    if (!muted) ma_sound_start(&soundStageAdvance);
}
