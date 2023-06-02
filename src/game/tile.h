#pragma once

#include "graphics/quad.h"

class Tile {

private:
    bool illuminated;

public:
    static const float SIZE;
    static const float INFECTION_DISABLED;

    float visTimerLastUpdate, visTimerShock, timerInfect;

    Quad quad;

    Tile(Tile const&) = delete;
    Tile& operator=(Tile const&) = delete;

    Tile();

    void setIlluminated(bool illuminatedArg);
    bool isIlluminated() const { return illuminated; }

    void flipIlluminated() { setIlluminated(!illuminated); }

};
