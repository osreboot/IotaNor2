#pragma once

#include "miniaudio/miniaudio.h"

// Some sounds can be played faster than their total runtime; so we need a queue of ma_sound instances to ensure that we
// can keep up with the number of requests.
class AudioQueue {

private:
    const int size;
    int indexCurrent;
    ma_sound* queue;

public:
    // The constructor fills the queue with 'size' number of ma_sound instances located at 'path' (including file name
    // and extension)
    AudioQueue(ma_engine& engine, int size, const char* path);
    ~AudioQueue();

    // Cycles the current sound instance to the least-recently-played instance
    void advance();

    // Returns a reference to the active ma_sound instance
    ma_sound& getCurrent();

};
