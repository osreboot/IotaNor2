
#include <chrono>

#include "game/render.h"
#include "graphics/display.h"
#include "graphics/painter.h"

typedef std::chrono::high_resolution_clock Clock;

int main() {
    display::initialize();
    painter::initialize();

    Clock::time_point timeLast = Clock::now();

    Game game;
    Render render(game);

    while(!display::isExiting()) {
        Clock::time_point timeNow = Clock::now();
        float delta = (float)std::chrono::duration_cast<std::chrono::nanoseconds>(timeNow - timeLast).count() / 1000000000.0f;
        timeLast = timeNow;

        display::preUpdate();

        game.update(delta);
        render.render(delta, game);

        display::postUpdate();
    }

    display::close();
    return 0;
}
