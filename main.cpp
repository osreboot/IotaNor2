#include <chrono>

#include "src/display.h"

using namespace std;
using namespace chrono;

int main() {
    display::initialize();

    high_resolution_clock::time_point timeLast = high_resolution_clock::now();

    while(!display::isExiting()) {
        high_resolution_clock::time_point timeNow = high_resolution_clock::now();
        float delta = (float)duration_cast<nanoseconds>(timeNow - timeLast).count() / 1000000000.0f;
        timeLast = timeNow;

        display::preUpdate();



        display::postUpdate();
    }

    display::close();
    return 0;
}
