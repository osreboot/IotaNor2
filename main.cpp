#include <chrono>
#include "src/display.h"
#include "src/painter.h"

using namespace std;
using namespace chrono;

int main() {
    display::initialize();
    painter::initialize();

    high_resolution_clock::time_point timeLast = high_resolution_clock::now();

    Quad quad(100.0f, 100.0f, 20.0f, 60.0f);

    while(!display::isExiting()) {
        high_resolution_clock::time_point timeNow = high_resolution_clock::now();
        float delta = (float)duration_cast<nanoseconds>(timeNow - timeLast).count() / 1000000000.0f;
        timeLast = timeNow;

        display::preUpdate();

        painter::draw(quad, 0, {1.0f, 0.0f, 0.0f, 1.0f});

        display::postUpdate();
    }

    display::close();
    return 0;
}
