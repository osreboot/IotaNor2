#include <chrono>

#include "src/display.h"
#include "src/painter.h"
#include "src/texture.h"

using namespace std;
using namespace chrono;

int main() {
    display::initialize();
    painter::initialize();

    high_resolution_clock::time_point timeLast = high_resolution_clock::now();

    Quad quad(100.0f, 100.0f, 512.0f, 512.0f);
    Quad quadCursor(0.0f, 0.0f, 60.0f, 60.0f);
    Texture texture("../res/tile_ref_2c.png");

    while(!display::isExiting()) {
        high_resolution_clock::time_point timeNow = high_resolution_clock::now();
        float delta = (float)duration_cast<nanoseconds>(timeNow - timeLast).count() / 1000000000.0f;
        timeLast = timeNow;

        display::preUpdate();

        pair<GLfloat, GLfloat> locationCursor = display::getCursor();
        quadCursor.x = locationCursor.first - (quadCursor.w / 2.0f);
        quadCursor.y = locationCursor.second - (quadCursor.h / 2.0f);

        painter::draw(quad, texture, {1.0f, 1.0f, 1.0f, 1.0f});
        painter::draw(quadCursor, texture, {1.0f, 0.0f, 1.0f, 1.0f});

        display::postUpdate();
    }

    display::close();
    return 0;
}
