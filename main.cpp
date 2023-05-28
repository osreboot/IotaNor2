#include <chrono>

#include "src/display.h"
#include "src/painter.h"
#include "src/texture.h"
#include "src/game/render.h"

using namespace std;
using namespace chrono;

int main() {
    display::initialize();
    painter::initialize();
    render::initialize();

    high_resolution_clock::time_point timeLast = high_resolution_clock::now();

    Game game;

    while(!display::isExiting()) {
        high_resolution_clock::time_point timeNow = high_resolution_clock::now();
        float delta = (float)duration_cast<nanoseconds>(timeNow - timeLast).count() / 1000000000.0f;
        timeLast = timeNow;

        display::preUpdate();

        game.update(delta);
        render::render(game);

        display::postUpdate();
    }

    render::cleanup();

    display::close();
    return 0;
}
