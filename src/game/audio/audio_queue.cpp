
#include "audio_queue.h"

AudioQueue::AudioQueue(ma_engine& engine, const int sizeArg, const char *path) :
    size(sizeArg), indexCurrent(0) {

    queue = new ma_sound[size];
    for (int i = 0; i < size; i++) {
        ma_sound_init_from_file(&engine, path, 0, nullptr, nullptr, &queue[i]);
    }
}

AudioQueue::~AudioQueue() {
    for (int i = 0; i < size; i++)
        ma_sound_uninit(&queue[i]);
    delete[] queue;
}

void AudioQueue::advance() {
    indexCurrent = (indexCurrent + 1) % size;
}

ma_sound& AudioQueue::getCurrent() {
    return queue[indexCurrent];
}
