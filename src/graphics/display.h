#pragma once

#include "types.h"

namespace display {

    void initialize();
    void preUpdate();
    void postUpdate();
    void close();

    bool isExiting();

    const Coordw& getSize();
    const Coordf& getCursor();

    bool hasEventMouseRelease();
    bool hasEventKeyPress(int key);
    bool hasEventKeyRelease(int key);

}
