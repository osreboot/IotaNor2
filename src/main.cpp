
#include <chrono>

#include "game/game.h"
#include "game/render.h"
#include "graphics/display.h"
#include "graphics/painter.h"

typedef std::chrono::high_resolution_clock Clock;

int main() {
    // GLFW and OpenGL initialization
    display::initialize();
    painter::initialize();

    Clock::time_point timeLast = Clock::now(); // Used to calculate per-tick deltas

    // Initialize game state
    Game game;
    Render render;

    while(!display::isExiting()) {
        // Calculate 'delta'; the time in seconds since the last update
        Clock::time_point timeNow = Clock::now();
        float delta = (float)std::chrono::duration_cast<std::chrono::nanoseconds>(timeNow - timeLast).count() / 1000000000.0f;
        timeLast = timeNow;

        display::preUpdate();

        // Update the game state and draw graphics
        game.update(delta);
        render.render(delta, game);

        display::postUpdate();
    }

    // The user has requested the application be closed, so exit
    display::close();
    return 0;
}
