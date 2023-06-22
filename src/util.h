#pragma once

#include <cmath>

// Re-maps the value 'x' from an input range to an output range (non-limiting)
inline float map(float x, float inMin, float inMax, float outMin, float outMax) {
    return (x - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}

// Returns the Euclidean distance between the points
inline float distance(float x1, float y1, float x2, float y2) {
    return sqrtf(powf(x1 - x2, 2.0f) + powf(y1 - y2, 2.0f));
}

// Advances 'x' towards 'goal' by 'delta', regardless of the sign of 'delta'
inline void stepTowards(float& x, float delta, float goal) {
    if (goal > x) {
        if (x + std::abs(delta) < goal) x = x + std::abs(delta); else x = goal;
    } else {
        if (x - std::abs(delta) > goal) x = x - std::abs(delta); else x = goal;
    }
}
