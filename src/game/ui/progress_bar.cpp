
#include "game/game.h"
#include "game/ui/progress_bar.h"
#include "game/render.h"
#include "graphics/display.h"
#include "graphics/painter.h"
#include "util.h"

const float ProgressBar::WIDTH = 256.0f;
const float ProgressBar::ASPECT = 1.0f / 16.0f;
const float ProgressBar::POS_Y = 48.0f;

ProgressBar::ProgressBar() :
        textureEnds(Texture::load("res/texture/ui/progress/progress_ends_c.png")),
        textureEndsBack(Texture::load("res/texture/ui/progress/progress_ends_cb.png")),
        textureCutout(Texture::load("res/texture/ui/progress/progress_cutout.png")),
        textureBlackGrow(Texture::load("res/texture/ui/progress/progress_black_grow.png")),
        textureWhiteGrow(Texture::load("res/texture/ui/progress/progress_white_grow.png")),
        textureBlackShrink(Texture::load("res/texture/ui/progress/progress_black_shrink.png")),
        textureWhiteShrink(Texture::load("res/texture/ui/progress/progress_white_shrink.png")),
        quad(0.0f, 0.0f, WIDTH, WIDTH * ASPECT),
        quadBlack(0.0f, 0.0f, WIDTH, WIDTH * ASPECT),
        quadWhite(0.0f, 0.0f, WIDTH, WIDTH * ASPECT),
        visProgress(0.0f) {}

void ProgressBar::draw(float delta, const Render& render, const Game &game) {
    // Calculate the percentage of illuminated tiles
    int numIlluminated = 0;
    for (int x = 0; x < Game::BOARD_DIM; x++) {
        for (int y = 0; y < Game::BOARD_DIM; y++) {
            if (game.tiles[x][y].isIlluminated()) numIlluminated++;
        }
    }
    float progress = static_cast<float>(numIlluminated) / static_cast<float>(Game::BOARD_DIM * Game::BOARD_DIM);

    // Step visual progress towards the real progress value
    stepTowards(visProgress, delta / 2.0f, progress);

    // Display background texture
    quad.x = (static_cast<float>(display::getSize().first) / 2.0f) - (WIDTH / 2.0f);
    quad.y = POS_Y - (WIDTH * ASPECT * 0.5f) * 0.75f;
    painter::draw(quad, textureEndsBack, render.shaderDefault, Render::WHITE);

    // Display right-side (white) arrow texture
    quadWhite.x = quad.x + (WIDTH * (1.0f - visProgress));
    quadWhite.y = quad.y;
    quadWhite.w = WIDTH * visProgress;
    quadWhite.u1 = quadWhite.w / WIDTH;
    painter::draw(quadWhite, game.getStageGoal() ? textureWhiteGrow : textureWhiteShrink, render.shaderDefault, Render::WHITE);

    // Display left-side (black) arrow texture
    quadBlack.x = quad.x;
    quadBlack.y = quad.y;
    quadBlack.w = WIDTH * (1.0f - visProgress);
    quadBlack.u0 = 1.0f - (quadBlack.w / WIDTH);
    painter::draw(quadBlack, game.getStageGoal() ? textureBlackShrink : textureBlackGrow, render.shaderDefault, Render::WHITE);

    // Display overlay textures
    painter::draw(quad, textureCutout, render.shaderDefault, Render::WHITE);
    painter::draw(quad, textureEnds, render.shaderDefault, Render::WHITE);
}
