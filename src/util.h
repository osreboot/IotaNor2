#pragma once

inline float map(float x, float inMin, float inMax, float outMin, float outMax) {
    return (x - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}

inline void stepTowards(float& x, float delta, float goal) {
    if (goal > x) {
        if (x + std::abs(delta) < goal) x = x + std::abs(delta); else x = goal;
    } else {
        if (x - std::abs(delta) > goal) x = x - std::abs(delta); else x = goal;
    }
}
