#pragma once

#include "miniaudio/miniaudio.h"

class AudioQueue {

private:
    const int size;
    int indexCurrent;
    ma_sound* queue;

public:
    AudioQueue(ma_engine& engine, int size, const char* path);
    ~AudioQueue();

    void advance();
    ma_sound& getCurrent();

};
